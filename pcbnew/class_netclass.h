/**
 * @file class_netclass.h
 */

/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2009 SoftPLC Corporation, Dick Hollenbeck <dick@softplc.com>
 * Copyright (C) 2009 Jean-Pierre Charras, jean-pierre.charras@inpg.fr
 * Copyright (C) 2009 KiCad Developers, see change_log.txt for contributors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */


#ifndef CLASS_NETCLASS_H
#define CLASS_NETCLASS_H


#include <macros.h>
#include <set>
#include <memory>
#include <bitset>
#include <richio.h>


class LINE_READER;
class BOARD;
class BOARD_DESIGN_SETTINGS;


DECL_SET_FOR_SWIG( STRINGSET, wxString )

typedef std::bitset<32> layerSelection;

/**
 * Class NETCLASS
 * handles a collection of nets and the parameters used to route or
 * test these nets.
 */

class NETCLASS
{
private:
    // Default values used to init a NETCLASS
    static const int DEFAULT_CLEARANCE;
    static const int DEFAULT_VIA_DRILL;
    static const int DEFAULT_UVIA_DRILL;
    static const int DEFAULT_VIA_DIAMETER;
    static const int DEFAULT_UVIA_DIAMETER;
    static const int DEFAULT_TRACK_WIDTH;
    static const int DEFAULT_DIFF_PAIR_WIDTH;
    static const int DEFAULT_DIFF_PAIR_GAP;
    static const int DEFAULT_MAX_VIAS;
    static const int DEFAULT_TOPOLOGY;
    static const int DEFAULT_MIN_LENGTH;
    static const int DEFAULT_MAX_LENGTH;
    static const int DEFAULT_MAX_SKEW;
    static const int DEFAULT_STUB_LENGTH;
    static const int DEFAULT_TYPE;
    layerSelection DEFAULT_LAYER_SELECTION;
    
    /**
     * There are four different types of signals that are known to the constraint manager.
     */
    enum {
        NO_TYPE,
        POWER_TYPE,
        SIGNAL_TYPE,
        MIXED_TYPE
    };
    
    /**
     * There are five different topologies known to the constraint manager.
     * 
     * The default topology for every signal is NO_SPECIAL_TOPOLOGY. There is no limitations on track routing.
     * STAR_TOPOLOGY means the there is only one start for many ends of the signal
     * T_TOPOLOGY means the signal is allowed to split into equally long branches
     * FLYBY_TOPOLOGY means the signal
     * HORIZONTAL_TOPOLOGY forces to route horizontally
     * VERTICAL_TOPOLOGY forces to route vertically
     * SIMPLE_DAISY_CHAIN_TOPOLOGY means the signal is routed from the start point to the next, to the next
     * MIDDRIVEN_DAISY_CHAIN_TOPOLOGY means the signal is routed from the middel of the length to each end point in two directions
     * MULTIPOINT_TOPOLOGY connects the nets in many places. Use this for GND and POWER nets
     */
    enum {
        NO_SPECIAL_TOPOLOGY,
        STAR_TOPOLOGY,
        T_TOPOLOGY,
        FLYBY_TOPOLOGY,
        HORIZONTAL_TOPOLOGY,
        VERTICAL_TOPOLOGY,
        SIMPLE_DAISY_CHAIN_TOPOLOGY,
        MIDDRIVEN_DAISY_CHAIN_TOPOLOGY,
        MULTIPOINT_TOPOLOGY
    };
        


protected:
    wxString    m_Name;                 ///< Name of the net class
    wxString    m_Description;          ///< what this NETCLASS is for.

    STRINGSET   m_Members;              ///< names of NET members of this class

    /// The units on these parameters is Internal Units (1 nm)

    int         m_Clearance;            ///< clearance when routing

    int         m_TrackWidth;           ///< track width used to route NETs in this NETCLASS
    int         m_ViaDia;               ///< via diameter
    int         m_ViaDrill;             ///< via drill hole diameter

    int         m_uViaDia;              ///< microvia diameter
    int         m_uViaDrill;            ///< microvia drill hole diameter

    int         m_diffPairWidth;
    int         m_diffPairGap;
    
    int         m_max_vias;             ///< number of vias from start to end of track 
    int         m_topology;             ///< routing topology
    
    int         m_min_length;           ///< minimal length of track from start to end
    int         m_max_length;           ///< maximum length of track from start to end
    int         m_max_skew;             ///< maximum skew of all tracks in class
    int         m_stub_length;          ///< stub length of track
    int         m_type;                 ///< routing type
    layerSelection    m_layer;                ///< each bit is a layer 1..32

public:

    static const char Default[];        ///< the name of the default NETCLASS

    /**
     * Constructor
     * stuffs a NETCLASS instance with aParent, aName, and optionally the initialParameters
     * @param aName = the name of this new netclass
     */
    NETCLASS( const wxString& aName );

    ~NETCLASS();

    wxString GetClass() const
    {
        return wxT( "NETCLASS" );
    }

    const wxString& GetName() const
    {
        return m_Name;
    }

    void SetName( const wxString& aName ) { m_Name = aName; }

    /**
     * Function GetCount
     * returns the number of nets in this NETCLASS, i.e. using these rules.
     */
    unsigned GetCount() const
    {
        return m_Members.size();
    }

    /**
     * Function Clear
     * empties the collection of members.
     */
    void Clear()
    {
        m_Members.clear();
    }

    /**
     * Function Add
     * adds \a aNetname to this NETCLASS if it is not already in this NETCLASS.
     * It is harmless to try and add a second identical name.
     */
    void Add( const wxString& aNetname )
    {
        m_Members.insert( aNetname );
    }

    typedef STRINGSET::iterator iterator;
    iterator begin() { return m_Members.begin(); }
    iterator end()   { return m_Members.end();   }

    typedef STRINGSET::const_iterator const_iterator;
    const_iterator begin() const { return m_Members.begin(); }
    const_iterator end()   const { return m_Members.end();   }

    /**
     * Function Remove
     * will remove NET name \a aName from the collection of members.
     */
    void Remove( iterator aName )
    {
        m_Members.erase( aName );
    }

    /**
     * Function Remove
     * will remove NET name \a aName from the collection of members.
     */
    void Remove( const wxString& aName )
    {
        m_Members.erase( aName );
    }

    STRINGSET& NetNames()                   { return m_Members; }       ///< for SWIG

    const wxString& GetDescription() const  { return m_Description; }
    void    SetDescription( const wxString& aDesc ) { m_Description = aDesc; }

    int     GetClearance() const            { return m_Clearance; }
    void    SetClearance( int aClearance )  { m_Clearance = aClearance; }

    int     GetTrackWidth() const           { return m_TrackWidth; }
    void    SetTrackWidth( int aWidth )     { m_TrackWidth = aWidth; }

    int     GetViaDiameter() const          { return m_ViaDia; }
    void    SetViaDiameter( int aDia )      { m_ViaDia = aDia; }

    int     GetViaDrill() const             { return m_ViaDrill; }
    void    SetViaDrill( int aSize )        { m_ViaDrill = aSize; }

    int     GetuViaDiameter() const         { return m_uViaDia; }
    void    SetuViaDiameter( int aSize )    { m_uViaDia = aSize; }

    int     GetuViaDrill() const            { return m_uViaDrill; }
    void    SetuViaDrill( int aSize )       { m_uViaDrill = aSize; }

    int     GetDiffPairWidth() const        { return m_diffPairWidth; }
    void    SetDiffPairWidth( int aSize )   { m_diffPairWidth = aSize; }

    int     GetDiffPairGap() const          { return m_diffPairGap; }
    void    SetDiffPairGap( int aSize )     { m_diffPairGap = aSize; }

    int     GetMaxVias() const              { return m_max_vias; }
    void    SetMaxVias( int aNumber )       { m_max_vias = aNumber; }
    
    int     GetTopology() const             { return m_topology; }
    void    SetTopology( int aTopology )    { m_topology = aTopology; }
    
    int     GetMinLength() const            { return m_min_length; }
    void    SetMinLength( int aLength )     { m_min_length = aLength; }
    
    int     GetMaxLength() const            { return m_max_length; }
    void    SetMaxLength( int aLength )     { m_max_length = aLength; }
    
    int     GetMaxSkew() const              { return m_max_skew; }
    void    SetMaxSkew( int aLength )       { m_max_skew = aLength; }
    
    int     GetStubLength() const           { return m_stub_length; }
    void    SetStubLength( int aLength )    { m_stub_length = aLength; }
    
    int     GetType() const                 { return m_type; }
    void    SetType( int aType )            { m_type = aType; }
    
    layerSelection    GetLayer() const             { return  m_layer; }
    void    SetLayer( layerSelection aLayer)    { m_layer = aLayer; }

    
    
    /**
     * Function SetParams
     * will set all the parameters by copying them from \a defaults.
     * Parameters are the values like m_ViaSize, etc, but do not include m_Description.
     * @param aDefaults is another NETCLASS object to copy from.
     */
    void SetParams( const NETCLASS& aDefaults );

    /**
     * Function Format
     * outputs the net class to \a aFormatter in s-expression form.
     *
     * @param aFormatter The #OUTPUTFORMATTER object to write to.
     * @param aNestLevel The indentation next level.
     * @param aControlBits The control bit definition for object specific formatting.
     * @throw IO_ERROR on write error.
     */
    void Format( OUTPUTFORMATTER* aFormatter, int aNestLevel, int aControlBits ) const;

#if defined(DEBUG)
    void Show( int nestLevel, std::ostream& os ) const;
#endif
};


DECL_SPTR_FOR_SWIG( NETCLASSPTR, NETCLASS )
DECL_MAP_FOR_SWIG( NETCLASS_MAP, wxString, NETCLASSPTR );


/**
 * Class NETCLASSES
 * is a container for NETCLASS instances.  It owns all its NETCLASSes
 * (=> it will delete them at time of destruction).  This container will always have
 * a default NETCLASS with the name given by const NETCLASS::Default.
 */
class NETCLASSES
{
private:

    /// all the NETCLASSes except the default one.
    NETCLASS_MAP    m_NetClasses;

public:
    NETCLASSES();
    ~NETCLASSES();

    /**
     * Function Clear
     * destroys any contained NETCLASS instances except the Default one.
     */
    void Clear()
    {
        m_NetClasses.clear();
    }

    typedef NETCLASS_MAP::iterator iterator;
    iterator begin() { return m_NetClasses.begin(); }
    iterator end()   { return m_NetClasses.end(); }

    typedef NETCLASS_MAP::const_iterator const_iterator;
    const_iterator begin() const { return m_NetClasses.begin(); }
    const_iterator end()   const { return m_NetClasses.end(); }

    /**
     * Function GetCount
     * @return the number of netclasses, excluding the default one.
     */
    unsigned GetCount() const
    {
        return m_NetClasses.size();
    }

    /**
     * Function GetDefault
     * @return the default net class.
     */
    NETCLASSPTR& GetDefault() const
    {
        static NETCLASSPTR defNetClass = std::make_shared<NETCLASS>( NETCLASS::Default );
        return defNetClass;
    }

    /**
     * Function Add
     * takes \a aNetclass and puts it into this NETCLASSES container.
     * @param aNetclass is netclass to add
     * @return true if the name within aNetclass is unique and it could be inserted OK,
     *  else false because the name was not unique.
     */
    bool Add( NETCLASSPTR aNetclass );

    /**
     * Function Remove
     * removes a NETCLASS from this container but does not destroy/delete it.
     * @param aNetName is the name of the net to delete, and it may not be NETCLASS::Default.
     * @return NETCLASSPTR - the NETCLASS associated with aNetName if found and removed, else NULL.
     */
    NETCLASSPTR Remove( const wxString& aNetName );

    /**
     * Function Find
     * searches this container for a NETCLASS given by \a aName.
     * @param aName is the name of the NETCLASS to search for.
     * @return NETCLASSPTR - if found, else NULL.
     */
    NETCLASSPTR Find( const wxString& aName ) const;

    /// Provide public access to m_NetClasses so it gets swigged.
    NETCLASS_MAP&   NetClasses()       { return m_NetClasses; }
};

#endif  // CLASS_NETCLASS_H

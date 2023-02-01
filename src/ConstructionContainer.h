#pragma once

#include <list>
#include <map>
#include <set>
#include <stdexcept>
#include <algorithm>

#include <iostream>



namespace Core
{
    template < typename T1, typename T2 >
    class ConstructionContainer;

    template < typename T1, typename T2 >
    std::ostream & operator<<( std::ostream &, ConstructionContainer< T1, T2 > const & );

    template < typename Unit, typename Body >
    class ConstructionContainer
    {
    private:
        struct UnitData
        {
        public:
            void AddBaseBody( Body const * body ) { m_baseBodies.push_back( body ); }
            void AddSlaveBody( Body const * body ) { m_slaveBodies.push_back( body ); }
            void RemoveBody( Body const * );

            std::list< Body const * > GetAllBodies() const;
            std::list< Body const * > const & GetBaseBodies() const { return m_baseBodies; }
            std::list< Body const * > const & GetSlaveBodies() const { return m_slaveBodies; }

            bool IsEmpty() const { return ( m_baseBodies.empty() && m_slaveBodies.empty() ); }
        private:
            std::list< Body const * > m_baseBodies;
            std::list< Body const * > m_slaveBodies;
        };

        struct BodyData
        {
        public:
            void AddBaseUnit( Unit const * unit ) { m_baseUnits.push_back( unit ); }
            void AddSlaveUnit( Unit const * unit ) { m_slaveUnits.push_back( unit ); }
            void RemoveUnit( Unit const * );

            std::list< Unit const * > GetAllUnits() const;

            // for tests
            std::list< Unit const * > const & GetBaseUnits() const { return m_baseUnits; }
            std::list< Unit const * > const & GetSlaveUnits() const { return m_slaveUnits; }
        private:
            std::list< Unit const * > m_baseUnits;
            std::list< Unit const * > m_slaveUnits;
        };

    public:
        void Add( Body const *, std::list< Unit const * > const & baseUnits );
        void AddUnitToBody( Unit const *, Body const * );
        void Add( Unit const * );
        void Remove( Unit const * );
        void Remove( Body const * );

        // // getters
        // std::list< Body * > GetSlaveBodies( Unit const * ) const;
        // std::list< Body * > GetBaseBodies( Unit const * ) const;
        // std::list< Unit * > GetSlaveUnits( Body const * ) const;
        // std::list< Unit * > GetBaseUnits( Body const * ) const;
        std::list< Body const * > GetAllBodies( Unit const * ) const;
        std::list< Unit const * > GetAllUnits( Body const * ) const;
        std::list< Body const * > GetAllBodies() const;
        std::list< Unit const * > GetIsolatedUnits() const;
        std::list< Unit const * > GetSlaveUnits( Body const * body ) const { return GetData( body ).GetSlaveUnits(); }

        // // questions

        friend std::ostream & operator<< <>( std::ostream &, ConstructionContainer< Unit, Body > const & );
    private:
        std::map< Unit const *, UnitData > m_unitsToData;
        std::map< Body const *, BodyData > m_bodiesToData;

        BodyData const & GetData( Body const * ) const;
        UnitData const & GetData( Unit const * ) const;
        // bool UnitIsConnectedWithOneBody( Unit * unit ) const;

        bool Contains( Unit const * ) const;
        bool Contains( Body const * ) const;

        void CleanUpData();
    };



    template < typename Unit, typename Body >
    void ConstructionContainer< Unit, Body >::Add( Unit const * unit )
    {
        if ( !unit )
        {
            throw std::invalid_argument( "ERROR : nullptr argument\n" );
        }

        if ( !Contains( unit ) )
        {
            m_unitsToData.insert( std::make_pair( unit, UnitData() ) );

            return;
        }

        throw std::invalid_argument( "ERROR : adding of an existing unit." );
    }

    template < typename Unit, typename Body >
    void ConstructionContainer< Unit, Body >::AddUnitToBody( Unit const * unit, Body const * body )
    {
        if ( !unit || !body )
        {
            throw std::invalid_argument( "ERROR : nullptr argument\n" );
        }

        if ( !Contains( body ) )
        {
            throw std::invalid_argument( "ERROR : adding a unit to not existing body." );
        }

        auto unitIter = m_unitsToData.find( unit );
        if ( unitIter != m_unitsToData.end() )
        {
            UnitData & data = unitIter->second;

            // if ( data.GetBaseBodies().size() > 0 )
            // {
            // 	throw std::invalid_argument( "ERROR : adding a unit to a body, where the unit has a base body." );
            // }

            std::list< Body const * > const & allBodies = data.GetAllBodies();
            //std::list< Body const * > const slaveBodies = data.GetSlaveBodies();
            if ( std::find( allBodies.begin(), allBodies.end(), body ) != allBodies.end() )
            {
                throw std::invalid_argument( "ERROR : adding a unit to its body." );
            }

            data.AddBaseBody( body );
        }
        else
        {
            UnitData data;
            data.AddBaseBody( body );

            m_unitsToData.insert( std::make_pair( unit, data ) );
        }

        auto bodyIter = m_bodiesToData.find( body );
        bodyIter->second.AddSlaveUnit( unit );
    }

    template < typename Unit, typename Body >
    void ConstructionContainer< Unit, Body >::Add( Body const * body, std::list< Unit const * > const & baseUnits )
    {
        if ( !body )
        {
            throw std::invalid_argument( "ERROR : nullptr argument\n" );
        }

        if ( Contains( body ) )
        {
            throw std::invalid_argument( "ERROR : adding an existing body." );
        }

        BodyData bodyData;
        for ( Unit const * unit : baseUnits )
        {
            if ( !unit )
            {
                throw std::invalid_argument( "ERROR : nullptr argument\n" );
            }

            auto unitIter = m_unitsToData.find( unit );
            if ( unitIter != m_unitsToData.end() )
            {
                unitIter->second.AddSlaveBody( body );
            }
            else
            {
                UnitData data;
                data.AddSlaveBody( body );

                m_unitsToData.insert( std::make_pair( unit, data ) );
            }

            bodyData.AddBaseUnit( unit );
        }

        m_bodiesToData.insert( std::make_pair( body, bodyData ) );
    }

    template < typename Unit, typename Body >
    void ConstructionContainer< Unit, Body >::Remove( Unit const * unit )
    {
        if ( !unit )
        {
            throw std::invalid_argument( "ERROR : nullptr argument\n" );
        }

        auto unitIter = m_unitsToData.find( unit );
        if ( unitIter == m_unitsToData.end() )
        {
            throw std::invalid_argument( "ERROR : removing a unit that doesn't exist in the container." );
        }

        std::set< Body const * > bodiesToRemove;
        std::set< Unit const * > unitsToRemove;
        unitsToRemove.insert( unit );

        std::set< Unit const * > currentUnitsToRemove;
        currentUnitsToRemove.insert( unit );
        std::set< Body const * > currentBodiesToRemove;
        while ( true )
        {
            if ( currentUnitsToRemove.empty() )
            {
                break;
            }

            for ( Unit const * unit : currentUnitsToRemove )
            {
                UnitData const & data = GetData( unit );
                std::list< Body const * > const slaveBodies = data.GetSlaveBodies();

                currentBodiesToRemove.insert( slaveBodies.begin(), slaveBodies.end() );

                bodiesToRemove.insert( slaveBodies.begin(), slaveBodies.end() );
            }

            currentUnitsToRemove.clear();

            for ( Body const * body : currentBodiesToRemove )
            {
                BodyData const & data = GetData( body );
                std::list< Unit const * > const slaveUnits = data.GetSlaveUnits();

                currentUnitsToRemove.insert( slaveUnits.begin(), slaveUnits.end() );

                unitsToRemove.insert( slaveUnits.begin(), slaveUnits.end() );
            }

            currentBodiesToRemove.clear();
        }

        for ( Unit const * unit : unitsToRemove )
        {
            m_unitsToData.erase( unit );
        }

        for ( Body const * body : bodiesToRemove )
        {
            m_bodiesToData.erase( body );
        }

        CleanUpData();
    }

    template < typename Unit, typename Body >
    void ConstructionContainer< Unit, Body >::Remove( Body const * body )
    {
        if ( !body )
        {
            throw std::invalid_argument( "ERROR : nullptr argument\n" );
        }

        auto bodyIter = m_bodiesToData.find( body );
        if ( bodyIter == m_bodiesToData.end() )
        {
            throw std::invalid_argument( "ERROR : removing a body that doesn't exist in the container." );
        }

        std::list< Unit const * > const slaveUnits = ( bodyIter->second ).GetSlaveUnits();
        for ( Unit const * unit : slaveUnits )
        {
            //std::cout << "Slave Unit : " << *unit << '\n';

            Remove( unit );
        }

        for ( Unit const * unit : bodyIter->second.GetBaseUnits() )
        {
            auto unitIter = m_unitsToData.find( unit );
            UnitData & data = unitIter->second;

            data.RemoveBody( body );
        }

        m_bodiesToData.erase( body );
    }

    template < typename Unit, typename Body >
    std::list< Body const * > ConstructionContainer< Unit, Body >::GetAllBodies() const
    {
        std::list< Body const * > result;

        for ( auto const & pair : m_bodiesToData )
        {
            result.push_back( pair.first );
        }

        return result;
    }

    template < typename Unit, typename Body >
    std::list< Unit const * > ConstructionContainer< Unit, Body >::GetIsolatedUnits() const
    {
        std::list< Unit const * > result;

        for ( auto const & pair : m_unitsToData )
        {
            if ( pair.second.IsEmpty() )
            {
                result.push_back( pair.first );
            }
        }

        return result;
    }

    template < typename Unit, typename Body >
    std::list< Body const * > ConstructionContainer< Unit, Body >::GetAllBodies( Unit const * unit ) const
    {
        //std::list< Body const * > result;
        //UnitData const & data = GetData( unit );

        return GetData( unit ).GetAllBodies();
    }

    template < typename Unit, typename Body >
    std::list< Unit const * > ConstructionContainer< Unit, Body >::GetAllUnits( Body const * body ) const
    {
        return GetData( body ).GetAllUnits();
    }



    template < typename Unit, typename Body >
    typename ConstructionContainer< Unit, Body >::UnitData const & ConstructionContainer< Unit, Body >::GetData( Unit const * unit ) const
    {
        auto iter = m_unitsToData.find( unit );

        return iter->second;
    }

    template < typename Unit, typename Body >
    typename ConstructionContainer< Unit, Body >::BodyData const & ConstructionContainer< Unit, Body >::GetData( Body const * body ) const
    {
        auto iter = m_bodiesToData.find( body );

        return iter->second;
    }

    template < typename Unit, typename Body >
    bool ConstructionContainer< Unit, Body >::Contains( Unit const * unit ) const
    {
        auto iter = m_unitsToData.find( unit );

        return ( iter != m_unitsToData.end() );
    }

    template < typename Unit, typename Body >
    bool ConstructionContainer< Unit, Body >::Contains( Body const * body ) const
    {
        auto iter = m_bodiesToData.find( body );

        return ( iter != m_bodiesToData.end() );
    }

    template < typename Unit, typename Body >
    void ConstructionContainer< Unit, Body >::CleanUpData()
    {
        for ( auto & pair : m_unitsToData )
        {
            UnitData & data = pair.second;

            std::list< Body const * > bodiesToRemove;
            for ( Body const * body : data.GetAllBodies() )
            {
                if ( !Contains( body ) )
                {
                    bodiesToRemove.push_back( body );
                }
            }

            for ( Body const * body : bodiesToRemove )
            {
                data.RemoveBody( body );
            }
        }

        for ( auto & pair : m_bodiesToData )
        {
            BodyData & data = pair.second;

            std::list< Unit const * > unitsToRemove;
            for ( Unit const * unit : data.GetAllUnits() )
            {
                if ( !Contains( unit ) )
                {
                    unitsToRemove.push_back( unit );
                }
            }

            for ( Unit const * unit : unitsToRemove )
            {
                data.RemoveUnit( unit );
            }
        }
    }



    template < typename Unit, typename Body >
    void ConstructionContainer< Unit, Body >::UnitData::RemoveBody( Body const * body )
    {
        m_baseBodies.remove( body );
        m_slaveBodies.remove( body );
    }

    template < typename Unit, typename Body >
    std::list< Body const * > ConstructionContainer< Unit, Body >::UnitData::GetAllBodies() const
    {
        std::list< Body const * > result( m_baseBodies );
        result.insert( result.end(), m_slaveBodies.begin(), m_slaveBodies.end() );

        return result;
    }

    template < typename Unit, typename Body >
    void ConstructionContainer< Unit, Body >::BodyData::RemoveUnit( Unit const * unit )
    {
        m_baseUnits.remove( unit );
        m_slaveUnits.remove( unit );
    }

    template < typename Unit, typename Body >
    std::list< Unit const * > ConstructionContainer< Unit, Body >::BodyData::GetAllUnits() const
    {
        std::list< Unit const * > result( m_baseUnits );
        result.insert( result.end(), m_slaveUnits.begin(), m_slaveUnits.end() );

        return result;
    }



    // template < typename Unit, typename Body >
    // std::ostream & ConstructionContainer< Unit, Body >::operator<<( std::ostream & os ) const
    // {
    // 	os << "[ConstructionContainer] :\n";

    // 	if ( m_unitsToData.empty() )
    // 	{
    // 		os << "Empty\n";

    // 		return os;
    // 	}

    // 	for ( auto pair : m_unitsToData )
    // 	{
    // 		Unit const * unit = pair.first;
    // 		UnitData const & data = pair.second;

    // 		os << "unit " << *unit << " :\n";

    // 		os << '\t' << "base bodies :\n";
    // 		for ( Body const * body : data.GetBaseBodies() )
    // 		{
    // 			os << "\t\t" << *body << '\n';
    // 		}

    // 		os << '\t' << "slave bodies :\n";
    // 		for ( Body const * body : data.GetSlaveBodies() )
    // 		{
    // 			os << "\t\t" << *body << '\n';
    // 		}
    // 	}

    // 	for ( auto pair : m_bodiesToData )
    // 	{
    // 		Body const * body = pair.first;
    // 		BodyData const & data = pair.second;

    // 		os << "body " << *body << " :\n";

    // 		os << '\t' << "base units :\n";
    // 		for ( Unit const * unit : data.GetBaseUnits() )
    // 		{
    // 			os << "\t\t" << *unit << '\n';
    // 		}

    // 		os << '\t' << "slave units :\n";
    // 		for ( Unit const * unit : data.GetSlaveUnits() )
    // 		{
    // 			os << "\t\t" << *unit << '\n';
    // 		}
    // 	}

    // 	return os;
    // }

    template < typename Unit, typename Body >
    std::ostream & operator<<( std::ostream & os,  ConstructionContainer< Unit, Body > const & container )
    {
        os << "[ConstructionContainer] :\n";

        if ( container.m_unitsToData.empty() )
        {
            os << "Empty\n";

            return os;
        }

        for ( auto pair : container.m_unitsToData )
        {
            Unit const * unit = pair.first;
            typename ConstructionContainer< Unit, Body >::UnitData const & data = pair.second;

            os << "unit " << *unit << " :\n";

            os << '\t' << "base bodies :\n";
            for ( Body const * body : data.GetBaseBodies() )
            {
                os << "\t\t" << *body << '\n';
            }

            os << '\t' << "slave bodies :\n";
            for ( Body const * body : data.GetSlaveBodies() )
            {
                os << "\t\t" << *body << '\n';
            }
        }

        for ( auto pair : container.m_bodiesToData )
        {
            Body const * body = pair.first;
            typename ConstructionContainer< Unit, Body >::BodyData const & data = pair.second;

            os << "body " << *body << " :\n";

            os << '\t' << "base units :\n";
            for ( Unit const * unit : data.GetBaseUnits() )
            {
                os << "\t\t" << *unit << '\n';
            }

            os << '\t' << "slave units :\n";
            for ( Unit const * unit : data.GetSlaveUnits() )
            {
                os << "\t\t" << *unit << '\n';
            }
        }

        return os;
    }








}



/* -*- c++ -*- */
/*
 * Copyright 2012 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#include <gnuradio/rpcserver_aggregator.h>
#include <gnuradio/rpcserver_booter_base.h>
#include <sstream>
#include <stdexcept>
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/assign/ptr_map_inserter.hpp>
#include <boost/foreach.hpp>

class rpcserver_aggregator::rpcserver_aggregator_impl {
private:
  friend class rpcserver_aggregator;
  rpcserver_aggregator_impl() : d_type("aggregator")
  {;}
  const std::string d_type;
  typedef boost::ptr_map<std::string, rpcmanager_base::rpcserver_booter_base_sptr> rpcServerMap_t;
  rpcServerMap_t d_registeredServers;
};

rpcserver_aggregator::rpcserver_aggregator() :
        p_impl(new rpcserver_aggregator_impl())
{;}

rpcserver_aggregator::~rpcserver_aggregator()
{;}

const std::string&
rpcserver_aggregator::type()
{
  return p_impl->d_type;
}

const std::vector<std::string>
rpcserver_aggregator::registeredServers()
{
  std::vector<std::string> keys;
  boost::copy(p_impl->d_registeredServers | boost::adaptors::map_keys, std::back_inserter(keys));
  return keys;
}

void
rpcserver_aggregator::registerConfigureCallback(const std::string &id,
                                                const configureCallback_t callback)
{
  BOOST_FOREACH(rpcserver_aggregator_impl::rpcServerMap_t::value_type booter, p_impl->d_registeredServers) {
    booter->second->get()->i()->registerConfigureCallback(id, callback);
  }
}

void
rpcserver_aggregator::unregisterConfigureCallback(const std::string &id)
{
  BOOST_FOREACH(rpcserver_aggregator_impl::rpcServerMap_t::value_type booter, p_impl->d_registeredServers) {
    booter->second->get()->i()->unregisterConfigureCallback(id);
  }
}

void
rpcserver_aggregator::registerQueryCallback(const std::string &id, const queryCallback_t callback)
{
  BOOST_FOREACH(rpcserver_aggregator_impl::rpcServerMap_t::value_type booter, p_impl->d_registeredServers) {
    booter->second->get()->i()->registerQueryCallback(id, callback);
  }
}

void
rpcserver_aggregator::unregisterQueryCallback(const std::string &id)
{
  BOOST_FOREACH(rpcserver_aggregator_impl::rpcServerMap_t::value_type booter, p_impl->d_registeredServers) {
    booter->second->get()->i()->unregisterQueryCallback(id);
  }
}

void
rpcserver_aggregator::registerServer(rpcmanager_base::rpcserver_booter_base_sptr server)
{
  if (p_impl->d_registeredServers.find(server->type()) == p_impl->d_registeredServers.end())
    {
      boost::assign::ptr_map_insert( p_impl->d_registeredServers )(server->type(), server);
    }
  else
    {
      std::stringstream s;
      s << "rpcserver_aggregator::registerServer: server of type "
          << server->type() << " already registered" << std::endl;
      throw std::runtime_error(s.str());
    }
}

void
rpcserver_aggregator::reconfigureServer(rpcmanager_base::rpcserver_booter_base_sptr server)
{
  if (p_impl->d_registeredServers.find(server->type()) != p_impl->d_registeredServers.end())
    {
      boost::assign::ptr_map_insert( p_impl->d_registeredServers )(server->type(), server);
    }
  else
    {
      std::stringstream s;
      s << "rpcserver_aggregator::registerServer: server of type "
          << server->type() << " has not registered" << std::endl;
      throw std::runtime_error(s.str());
    }
}

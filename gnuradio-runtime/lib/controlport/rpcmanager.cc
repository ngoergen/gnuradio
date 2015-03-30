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

#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/assign/ptr_map_inserter.hpp>
#include <boost/foreach.hpp>
#include <gnuradio/rpcserver_booter_aggregator.h>
#include <gnuradio/rpcmanager.h>
#include <iostream>
#include <stdexcept>

class rpcmanager::rpcmanager_impl : private boost::noncopyable {
private:
  rpcmanager_impl() :
    booter_registered(false),
    aggregator_registered(false),
    boot(),
    aggregator()
  {;}

  friend class rpcmanager;

  bool booter_registered;
  bool aggregator_registered;
  rpcmanager_base::rpcserver_booter_base_sptr boot;
  rpcserver_booter_aggregator::rpcserver_booter_aggregator_sptr aggregator;

  typedef boost::ptr_map<std::string,rpcserver_booter_factory_base_sptr> rpcBooter_factory_map_t;
  rpcBooter_factory_map_t d_registered_booter_factories;
};

boost::scoped_ptr<rpcmanager::rpcmanager_impl>rpcmanager::p_impl;

rpcmanager::rpcmanager() {;}

rpcmanager::~rpcmanager() {;}

void rpcmanager::initialization_check() {
  if(!p_impl)
    p_impl.reset(new rpcmanager_impl());
}

rpcserver_booter_base*
rpcmanager::get()
{
  initialization_check();

  if(p_impl->aggregator_registered)
  {
      return p_impl->aggregator.get();
  }
  else if(p_impl->booter_registered) 
  {
      return p_impl->boot.get();
  }
  assert(p_impl->booter_registered || p_impl->aggregator_registered);
  return p_impl->boot.get();
}

void
rpcmanager::register_booter(rpcmanager_base::rpcserver_booter_base_sptr booter)
{
  initialization_check();

  if(make_aggregator && !p_impl->aggregator_registered) {
      p_impl->aggregator.reset(new rpcserver_booter_aggregator());
      p_impl->aggregator_registered = true;
  }

  if(p_impl->aggregator_registered)
    {
      p_impl->aggregator->instance()->registerServer(booter);
    }
  else if(!p_impl->booter_registered)
    {
      p_impl->boot = booter;
      p_impl->booter_registered = true;
    }
  else {
      throw std::runtime_error("rpcmanager: Aggregator not in use, and a rpc booter is already registered\n");
  }
}

void
rpcmanager::register_booter_factory(const std::string& booter_type, rpcserver_booter_factory_base_sptr booter_factory) {
  initialization_check();

  if (p_impl->d_registered_booter_factories.find(
      booter_type) == p_impl->d_registered_booter_factories.end())
    {
      boost::assign::ptr_map_insert( p_impl->d_registered_booter_factories )(booter_type, booter_factory);
    }
  else
    {
      std::stringstream s;
      s << "rpcserver_aggregator::register_booter_factory: booter factory of type "
          << booter_type << " already registered" << std::endl;
      throw std::runtime_error(s.str());
    }
}

void
rpcmanager::reset_booter(rpcmanager_base::rpcserver_booter_base_sptr booter)
{
  initialization_check();

  if(p_impl->aggregator_registered)
    {
      p_impl->aggregator->instance()->reconfigure_server(booter);
    }
  else if(p_impl->booter_registered)
    {
      p_impl->boot = booter;
    }
  else {
      throw std::runtime_error("rpcmanager: reset_booter called, but an aggregator is not in use, "
          "and a booter is not currently registered\n");
  }
  std::cout << " booter->i();" << std::endl;
  booter->i();
}

void
rpcmanager::reset_booter(const std::string& booter_type) {
  initialization_check();

  rpcmanager_impl::rpcBooter_factory_map_t::iterator booter_factory(
      p_impl->d_registered_booter_factories.find(booter_type));
  if (booter_factory != p_impl->d_registered_booter_factories.end())
    {
      rpcmanager::reset_booter(booter_factory->second->get()->create());
    }
  else
    {
      std::stringstream s;
      s << "rpcserver_aggregator::register_booter_factory: booter factory of type "
          << booter_type << " not registered" << std::endl;
      throw std::runtime_error(s.str());
    }
}

void
rpcmanager::reset_all_booters() {
  initialization_check();

  BOOST_FOREACH(rpcmanager_impl::rpcBooter_factory_map_t::value_type booter_factory, p_impl->d_registered_booter_factories) {
    rpcmanager::reset_booter(booter_factory->second->get()->create());
  }
}

void
rpcmanager::stop_server(const std::string& server_type) {
  initialization_check();

  if(p_impl->aggregator_registered)
    {
      p_impl->aggregator->instance()->stop_server(server_type);
    }
  else if(p_impl->booter_registered)
    {
      p_impl->boot->stop_server(server_type);
    }
  else {
      throw std::runtime_error("rpcmanager: stop_server called, but an aggregator is not in use, "
          "and a booter is not currently registered\n");
  }
}

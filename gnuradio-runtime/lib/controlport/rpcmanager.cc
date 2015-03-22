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

  if(p_impl->aggregator_registered) {
      return p_impl->aggregator.get();
  }
  else if(p_impl->booter_registered) {
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
rpcmanager::reconfigure_booter(rpcmanager_base::rpcserver_booter_base_sptr booter)
{
  initialization_check();

  if(p_impl->aggregator_registered)
    {
      p_impl->aggregator->instance()->reconfigureServer(booter);
    }
  else if(p_impl->booter_registered)
    {
      p_impl->boot = booter;
    }
  else {
      throw std::runtime_error("rpcmanager: Reconfigure called, but an aggregator is not in use, "
          "and a booter is not currently registered\n");
  }
}

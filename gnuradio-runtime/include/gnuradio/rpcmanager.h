/* -*- c++ -*- */
/*
 * Copyright 2012,2015 Free Software Foundation, Inc.
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

#ifndef RPCMANAGER_H
#define RPCMANAGER_H

#include <gnuradio/api.h>
#include <gnuradio/rpcmanager_base.h>
#include <gnuradio/rpcserver_booter_aggregator.h>
#include <boost/scoped_ptr.hpp>
#include <iostream>

class GR_RUNTIME_API rpcmanager : public virtual rpcmanager_base
{
public:
  rpcmanager();
  ~rpcmanager();

  static rpcserver_booter_base* get();

  static void register_booter(rpcmanager_base::rpcserver_booter_base_sptr booter);

  static void reconfigure_booter(rpcmanager_base::rpcserver_booter_base_sptr booter);

  template<typename T> class rpcserver_booter_register_helper
  {
  public:
    rpcserver_booter_register_helper()
  {
      rpcmanager::register_booter(create());
  }

    void reconfigure()
    {
      rpcmanager::reconfigure_booter(create());
    }

  private:
    rpcmanager_base::rpcserver_booter_base_sptr create() {
      return rpcmanager_base::rpcserver_booter_base_sptr(new T());
    }

    //TODO: unregister
  };

private:
  class rpcmanager_impl;
  static boost::scoped_ptr<rpcmanager_impl> p_impl;

  static void initialization_check();
  static bool make_aggregator;
};

#endif /* RPCMANAGER_H */

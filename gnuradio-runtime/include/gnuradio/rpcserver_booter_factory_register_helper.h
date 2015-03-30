/* -*- c++ -*- */
/*
 * Copyright 2015 Free Software Foundation, Inc.
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

#ifndef RPCSERVER_BOOTER_FACTORY_REGISTER_HELPER
#define RPCSERVER_BOOTER_FACTORY_REGISTER_HELPER

#include <gnuradio/rpcserver_booter_register_helper.h>
#include <gnuradio/rpcserver_booter_factory.h>

template<typename T>
class rpcserver_booter_factory_register_helper
{
public:
  rpcserver_booter_factory_register_helper();
private:
  rpcmanager_base::rpcserver_booter_factory_base_sptr d_factory;
  rpcmanager_base::rpcserver_booter_base_sptr d_booter;
  rpcserver_booter_register_helper d_initial_booter;
};

template<typename T>
rpcserver_booter_factory_register_helper<T>::rpcserver_booter_factory_register_helper():
  d_factory(new rpcserver_booter_factory<T>()),
  d_booter(d_factory->create()),
  d_initial_booter(d_booter)
{
  rpcmanager::register_booter_factory(d_booter->type(), d_factory);
}

#endif /* RPCSERVER_BOOTER_FACTORY_REGISTER_HELPER */

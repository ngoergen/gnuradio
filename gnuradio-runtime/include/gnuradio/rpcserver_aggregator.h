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

#ifndef RPCSERVER_AGGREGATOR_H
#define RPCSERVER_AGGREGATOR_H

#include <string>
#include <boost/scoped_ptr.hpp>
#include <gnuradio/rpcserver_base.h>
#include <gnuradio/rpcmanager_base.h>

class rpcserver_aggregator : public virtual rpcserver_base
{
public:
  rpcserver_aggregator();
  virtual ~rpcserver_aggregator();

  void registerConfigureCallback(const std::string &id, const configureCallback_t callback);
  void unregisterConfigureCallback(const std::string &id);

  void registerQueryCallback(const std::string &id, const queryCallback_t callback);
  void unregisterQueryCallback(const std::string &id);

  void registerServer(rpcmanager_base::rpcserver_booter_base_sptr server);
  void reconfigureServer(rpcmanager_base::rpcserver_booter_base_sptr server);

  const std::string& type();

  const std::vector<std::string> registeredServers();

private:
  class rpcserver_aggregator_impl;
  boost::scoped_ptr<rpcserver_aggregator_impl> p_impl;
};

#endif /* RPCSERVER_AGGREGATOR_H */

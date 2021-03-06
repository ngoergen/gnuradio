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

#ifndef THRIFT_SERVER_TEMPLATE_H
#define THRIFT_SERVER_TEMPLATE_H

#include <gnuradio/rpcserver_thrift.h>
#include <gnuradio/thrift_application_base.h>
#include <iostream>

#include <thrift/Thrift.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <ControlPort.h>

using namespace apache;

template<typename TserverBase, typename TserverClass, typename TImplClass, typename TThriftClass>
class thrift_server_template : public thrift_application_base<TserverBase, TImplClass>
{
public:
  thrift_server_template(TImplClass* _this,
                         const std::string& contolPortName,
                         const std::string& endpointName);
  ~thrift_server_template();

protected:
  TserverBase* i_impl();
  friend class thrift_application_base<TserverBase, TImplClass>;

  TserverBase* d_server;
  const std::string d_contolPortName, d_endpointName;
};

template<typename TserverBase, typename TserverClass, typename TImplClass, typename TThriftClass>
thrift_server_template<TserverBase, TserverClass, TImplClass, TThriftClass>::thrift_server_template
(TImplClass* _this, const std::string& controlPortName, const std::string& endpointName)
  : thrift_application_base<TserverBase, TImplClass>(_this),
    d_contolPortName(controlPortName),
    d_endpointName(endpointName)
{
  //std::cerr << "thrift_server_template: ctor" << std::endl;

  boost::shared_ptr<TserverClass> handler(new TserverClass());

  boost::shared_ptr<thrift::TProcessor>
    processor(new GNURadio::ControlPortProcessor(handler));

  boost::shared_ptr<thrift::transport::TServerTransport>
    serverTransport(new thrift::transport::TServerSocket(9090));

  boost::shared_ptr<thrift::transport::TTransportFactory>
    transportFactory(new thrift::transport::TBufferedTransportFactory());

  boost::shared_ptr<thrift::protocol::TProtocolFactory>
    protocolFactory(new thrift::protocol::TBinaryProtocolFactory());

  thrift_application_base<TserverBase, TImplClass>::d_thriftserver =
    new thrift::server::TSimpleServer(processor, serverTransport, transportFactory, protocolFactory);

  d_server = (TserverBase*)handler.get();
}

template<typename TserverBase, typename TserverClass, typename TImplClass, typename TThriftClass>
thrift_server_template<TserverBase, TserverClass,TImplClass, TThriftClass>::~thrift_server_template()
{
}

template<typename TserverBase, typename TserverClass, typename TImplClass, typename TThriftClass>
TserverBase* thrift_server_template<TserverBase, TserverClass, TImplClass, TThriftClass>::i_impl()
{
  //std::cerr << "thrift_server_template: i_impl" << std::endl;
  return d_server;
}

#endif /* THRIFT_SERVER_TEMPLATE_H */

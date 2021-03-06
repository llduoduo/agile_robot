/*
 * fake_can.h
 *
 *  Created on: Jul 11, 2018
 *      Author: bibei
 */

#ifndef INCLUDE_PLATFORM_PROPAGATE_FAKE_CAN_H_
#define INCLUDE_PLATFORM_PROPAGATE_FAKE_CAN_H_

#include "propagate.h"
#include <vector>

namespace agile_robot {

class FakeCan: public Propagate {
public:
  FakeCan();
  virtual bool auto_init() override;

  virtual ~FakeCan();

public:
  virtual bool start() override;
  virtual void stop()  override;

  virtual bool write(const Packet&) override;
  virtual bool read(Packet&)        override;

private:
  ///! screen or NULL;
  std::string         output_;
  std::vector<Packet> pkt_from_;
};

} /* namespace agile_robot */

#endif /* INCLUDE_PLATFORM_PROPAGATE_FAKE_CAN_H_ */

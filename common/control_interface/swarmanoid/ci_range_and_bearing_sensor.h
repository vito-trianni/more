/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 * @file common/control_interface/ci_range_and_bearing_sensor.h
 *
 * @brief This file provides the definition of the sensor part of the
 *        range and bearing system.
 *
 * The range and bearing sensor/actuator pair allows robots to
 * sense other robots in their locality and exchange data with them.
 * When the presence of a robot cannot be sensed anymore (i.e., the sensing
 * robot does not receive an IR beam from the robot), the data sent
 * over the local radio is ignored. Thus, this sensor/actuator pair
 * provides localization and local communication to the robot.
 * The range of the sensor depends on the robot type.
 *
 * For each sensed robot, the following data is provided:
 *
 *  1) id of the robot sensed
 *  2) range (=distance) in meters relative to the sensing robot
 *  3) bearing horizontal (=angle) in signed radians relative to the sensing robot
 *  4) bearing vertical   (=angle) in signed radians relative to the sensing robot
 *      -> this values is currently not supported and therefore always 0
 *  5) data (10 bytes) transmitted by the other robot
 *
 * All this data is encapsulated in a TRangeAndBearingReceivedPacket struct.
 *
 * The sensor provides a map that holds the last received packet for each robot.
 * The key to the map is the robot's id, so that calling controllers can request
 * the data on a per-robot basis. The map can be accessed by calling the
 * CCI_RangeAndBearingSensor::GetLastReceivedPackets() method. For example,
 * getting the most recent message we got from the robot with the id "example-footbot",
 * the controller would call m_cRangeAndBearingSensor::GetLastReceivedPackets()["example-footbot"],
 * which would return the corresponding packed (a TRangeAndBearingReceivedPacket).
 *
 * Additionally to the data described above, each packet is assigned a unique,
 * sequentially increased number upon reception. Together with the method
 * CCI_RangeAndBearingSensor::GetLatestPacketId(), controllers can check if
 * there are new packets arrived since the last call and can iterate over
 * the list of packages.
 *
 * @author Arne Brutschy - <arne.brutschy@ulb.ac.be>
 */

#ifndef CCI_RANGE_AND_BEARING_SENSOR_H
#define CCI_RANGE_AND_BEARING_SENSOR_H

/* To avoid dependency problems when including */
namespace argos {
   class CCI_RangeAndBearingSensor;
}

#include <string>
#include <vector>
#include <iostream>

#include <argos2/common/control_interface/ci_sensor.h>
#include <argos2/common/utility/math/angles.h>

namespace argos {

   /**
    * This struct defines a received range and bearing packet.
    * It holds the id of the sending robot, the distance (in meters), the horizontal
    * and vertical angle (signed radians), the packet payload data (10 bytes) and
    * a packet id, that is sequentially increased each time a packet is received.
    *
    * The vertical angle is always zero in case of the hand-bot and the foot-bot.
    *
    * Please note that this packet is only used to hold data on the receiving
    * end of the communication.
    */
   struct TRangeAndBearingReceivedPacket {

      typedef UInt8 TRangeAndBearingData[10];
      typedef UInt16 TRawValues[12];

      /* The robot's id. */
      std::string RobotId;

      /* Distance in meters. */
      Real Range;

      /* Horizontal angle, in radians. */
      CRadians BearingHorizontal;

      /* Vertical angle, in radians. */
      CRadians BearingVertical;

      /* Datatype for the range and bearing payload. */
      TRangeAndBearingData Data;

      /* Raw values used for the calibration of the 3D bearing. Don't remove */
      TRawValues RawValues;

      /* The id of the packet. */
      UInt64 Id;

      /* Constructor. */
      TRangeAndBearingReceivedPacket() :
         Range(0.0), BearingHorizontal(0.0), BearingVertical(0.0), Id(0) {
      }

      TRangeAndBearingReceivedPacket(const TRangeAndBearingReceivedPacket& t_packet) :
         RobotId(t_packet.RobotId), Range(t_packet.Range),
         BearingHorizontal(t_packet.BearingHorizontal),
         BearingVertical(t_packet.BearingVertical), Id(t_packet.Id) {
         ::memcpy(Data, &t_packet.Data, sizeof(TRangeAndBearingData));
         ::memcpy(RawValues, &t_packet.RawValues, sizeof(TRawValues));
      }

      TRangeAndBearingReceivedPacket& operator=(const TRangeAndBearingReceivedPacket& t_packet) {
         if (&t_packet != this) {
            RobotId = t_packet.RobotId;
            Range = t_packet.Range;
            BearingHorizontal = t_packet.BearingHorizontal;
            BearingVertical = t_packet.BearingVertical;
            Id = t_packet.Id;
            ::memcpy(Data, &t_packet.Data, sizeof(TRangeAndBearingData));
            ::memcpy(RawValues, &t_packet.RawValues, sizeof(TRawValues));
         }
         return *this;
      }

      friend std::ostream& operator<<(std::ostream& os,
                                      const TRangeAndBearingReceivedPacket& t_packet) {
         os << "RANGE_AND_BEARING_RECEIVED_DATA < id = " << t_packet.RobotId
            << ", range = " << t_packet.Range
            << ", bearing horizontal = " << t_packet.BearingHorizontal
            << ", bearing vertical = " << t_packet.BearingVertical
            << ", data = ["
            << t_packet.Data[0] << ":"
            << t_packet.Data[1] << ":"
            << t_packet.Data[2] << ":"
            << t_packet.Data[3] << ":"
            << t_packet.Data[4] << ":"
            << t_packet.Data[5] << ":"
            << t_packet.Data[6] << ":"
            << t_packet.Data[7] << ":"
            << t_packet.Data[8] << ":"
            << t_packet.Data[9]
            << "], id = " << t_packet.Id
            << " >";

         return os;
      }
   };

   class CCI_RangeAndBearingSensor : public CCI_Sensor {

   public:

      /* Type for the map that holds the latest received packet for each robot. */
      typedef std::map<std::string, TRangeAndBearingReceivedPacket> TLastReceivedPackets;

      CCI_RangeAndBearingSensor() :
         m_unLatestPacketId(0) {
      }
      virtual ~CCI_RangeAndBearingSensor() {
      }

      /* Clears the messages received from the range and bearing. Call this at the end of your
       * time-step to be sure that at each time-step you only have the most recently received packets*/
      inline virtual void ClearRABReceivedPackets() {
         m_tLastReceivedPackets.clear();
      }

      /* Get the latest packet for all robots. Might contain empty packets. */
      inline virtual const TLastReceivedPackets& GetLastReceivedPackets() const {
         return m_tLastReceivedPackets;
      }

      /* Get the id of the packet received most recently. */
      inline virtual UInt64 GetLatestPacketId() const {
         return m_unLatestPacketId;
      }

      virtual UInt16 GetRabIdFromRobotId(const std::string& str_robot_id) = 0;

      virtual const std::string& GetRobotIdFromRabId(UInt16 un_rab_id) = 0;

   protected:

      /* Stores the last received packet from each robot. */
      TLastReceivedPackets m_tLastReceivedPackets;

      /* Id of the latest received packet. Can be used to get the freshly arrived packages only. */
      UInt64 m_unLatestPacketId;

   };

}

#endif

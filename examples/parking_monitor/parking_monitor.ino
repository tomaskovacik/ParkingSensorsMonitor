#include <ParkSensMon.h>

ParkSensMon PDC(6);

void setup()
{
  PDC.begin();
  Serial.begin(115200);
}

void loop()
{
  if (PDC.available() /*&& (PDC.getDistance(1)<0xFF || PDC.getDistance(2)<0xFF || PDC.getDistance(3)<0xFF || PDC.getDistance(4)<0xFF)*/) {
    Serial.print((PDC.getDistance(1)<0xFF)?String(PDC.getDistanceInMeters(1), 1)+"m ":"  ");
    Serial.print((PDC.getDistance(2)<0xFF)?String(PDC.getDistanceInMeters(2), 1)+"m ":"  ");
    Serial.print((PDC.getDistance(3)<0xFF)?String(PDC.getDistanceInMeters(3), 1)+"m ":"  ");
    Serial.print((PDC.getDistance(4)<0xFF)?String(PDC.getDistanceInMeters(4), 1)+"m ":"  ");
    Serial.println();
    PDC.clearNewData();
  }
}

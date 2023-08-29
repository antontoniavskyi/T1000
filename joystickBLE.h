#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

#define bleServerName "T1000_joysticks"

#define JS1_SERVICE_UUID        "a9802ac6-6ed9-4815-83f2-edd01aa31e13"
#define JS1_X_CHARACTERISTIC_UUID "6ac33036-b0ea-48f2-b780-5d8b25f2616a"
#define JS1_Y_CHARACTERISTIC_UUID "522bb4f1-913a-48e4-b0b9-1330268017b4"
#define JS1_BT_CHARACTERISTIC_UUID "ed429f11-1910-49bf-b49c-7731f61818fd"


#define JS2_SERVICE_UUID        "088fe84c-504a-4495-a29f-6a78350b726e"
#define JS2_X_CHARACTERISTIC_UUID "6c0b9fde-7cf9-49a6-a96d-5b292646b3cd"
#define JS2_Y_CHARACTERISTIC_UUID "d7370e6c-0a6a-4507-a4e6-f76a856f1f5a"
#define JS2_BT_CHARACTERISTIC_UUID "c88da022-5b31-4034-98e8-375c7806ce74"

bool deviceConnected = false;

BLECharacteristic joystickXCharacteristic_1(JS1_X_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_NOTIFY);
BLECharacteristic joystickYCharacteristic_1(JS1_Y_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_NOTIFY);
BLECharacteristic joystickBTCharacteristic_1(JS1_BT_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_NOTIFY);

BLECharacteristic joystickXCharacteristic_2(JS2_X_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_NOTIFY);
BLECharacteristic joystickYCharacteristic_2(JS2_Y_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_NOTIFY);
BLECharacteristic joystickBTCharacteristic_2(JS2_BT_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_NOTIFY);

BLEDescriptor joystickDescriptor(BLEUUID((uint16_t)0x2902));

//Setup callbacks onConnect and onDisconnect
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };
    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

void joystickBLEinit() {
  BLEDevice::init(bleServerName);
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *joystickService_1 = pServer->createService(JS1_SERVICE_UUID);
  BLEService *joystickService_2 = pServer->createService(JS2_SERVICE_UUID);

  joystickService_1->addCharacteristic(&joystickXCharacteristic_1);
  joystickService_1->addCharacteristic(&joystickYCharacteristic_1);
  joystickService_1->addCharacteristic(&joystickBTCharacteristic_1);

  joystickService_2->addCharacteristic(&joystickXCharacteristic_2);
  joystickService_2->addCharacteristic(&joystickYCharacteristic_2);
  joystickService_2->addCharacteristic(&joystickBTCharacteristic_2);

  joystickDescriptor.setValue("Joystick 1");

  joystickXCharacteristic_1.addDescriptor(&joystickDescriptor);
  joystickYCharacteristic_1.addDescriptor(&joystickDescriptor);
  joystickBTCharacteristic_1.addDescriptor(&joystickDescriptor);

  joystickXCharacteristic_2.addDescriptor(&joystickDescriptor);
  joystickYCharacteristic_2.addDescriptor(&joystickDescriptor);
  joystickBTCharacteristic_2.addDescriptor(&joystickDescriptor);

  joystickService_1->start();
  joystickService_2->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(JS1_SERVICE_UUID);
  pAdvertising->addServiceUUID(JS2_SERVICE_UUID);
  pServer->getAdvertising()->start();
  BLEDevice::startAdvertising();
}

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

#define bleServerName "T1000_joysticks"

#define JS1_SERVICE_UUID        "a9802ac6-6ed9-4815-83f2-edd01aa31e13"
#define JS1_X_CHARACTERISTIC_UUID "6ac33036-b0ea-48f2-b780-5d8b25f2616a"
#define JS1_Y_CHARACTERISTIC_UUID "522bb4f1-913a-48e4-b0b9-1330268017b4"
#define JS1_BT_CHARACTERISTIC_UUID "ed429f11-1910-49bf-b49c-7731f61818fd"

bool deviceConnected = false;

BLECharacteristic joystickXCharacteristic(JS1_X_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_NOTIFY);
BLECharacteristic joystickYCharacteristic(JS1_Y_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_NOTIFY);
BLECharacteristic joystickBTCharacteristic(JS1_BT_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_NOTIFY);

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

void joystickBLEinit(){
  BLEDevice::init(bleServerName);
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *joystickService = pServer->createService(JS1_SERVICE_UUID);

  joystickService->addCharacteristic(&joystickXCharacteristic);
  joystickService->addCharacteristic(&joystickYCharacteristic);
  joystickService->addCharacteristic(&joystickBTCharacteristic);
  joystickDescriptor.setValue("Joystick 1");
  joystickXCharacteristic.addDescriptor(&joystickDescriptor);
  joystickYCharacteristic.addDescriptor(&joystickDescriptor);
  joystickBTCharacteristic.addDescriptor(&joystickDescriptor);
  joystickService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(JS1_SERVICE_UUID);
  pServer->getAdvertising()->start();
  BLEDevice::startAdvertising();
}

using System;
using System.Collections.Generic;
using System.Text;
using Manzana;
namespace IphoneReadLib
{
    public class DeviceInfo
    {
        public string DeviceName;
        public string DeviceId;
        public string DeviceType;
        public string DeviceVersion;
        public string DeviceSerial;
        public string IMEI;
        public string ActivationState;
        public bool IsConnected;
    }
    public class CIphoneRead
    {
        private static iPhone iPhoneInterface = new iPhone();

        public static DeviceInfo GetDeviceInfo()
        {
            DeviceInfo device = new DeviceInfo();
            device.IsConnected = iPhoneInterface.IsConnected;
            if (!device.IsConnected)
            {
                return device;
            }
            device.DeviceName = iPhoneInterface.DeviceName;
            device.DeviceId = iPhoneInterface.DeviceId;
            device.DeviceType = iPhoneInterface.DeviceType;
            device.DeviceVersion = iPhoneInterface.DeviceVersion;
            device.DeviceSerial = iPhoneInterface.DeviceSerial; 
            device.IMEI = iPhoneInterface.InternationalMobileEquipmentIdentity;
            device.ActivationState = iPhoneInterface.ActivationState; 

            return device;
        }
    }
}

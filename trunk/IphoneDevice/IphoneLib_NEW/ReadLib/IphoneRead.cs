using System;
using System.Collections.Generic;
using System.Text;
using Manzana;
using System.IO;
using Microsoft.Win32;
namespace IphoneReadLib
{
    public class DeviceInfo
    {
        public string ModelNumber;
        public string ICCID;
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
            device.ICCID = iPhoneInterface.GetIntegratedCircuitCardIdentity;
            device.DeviceName = iPhoneInterface.GetDeviceName;
            device.DeviceId = iPhoneInterface.GetUniqueDeviceID;
            device.DeviceType = iPhoneInterface.GetProductType;
            device.DeviceVersion = iPhoneInterface.GetDeviceVersion;
            device.DeviceSerial = ""; 
            device.IMEI = iPhoneInterface.GetInternationalMobileEquipmentIdentity;
            device.ActivationState = iPhoneInterface.GetActivationState;
            device.ModelNumber = iPhoneInterface.GetModelNumber;
            return device;
        }
    }
}

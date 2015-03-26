using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using IphoneReadLib;
namespace iTunesDLLDebug
{
    public partial class frmMain : Form
    {
        
        public frmMain()
        {
            InitializeComponent();
        }

        private void frmMain_Load(object sender, EventArgs e)
        {            
        }
        /*
                    Valid Value Names:
                            ActivationState
                            ActivationStateAcknowledged
                            BasebandBootloaderVersion
                            BasebandVersion
                            BluetoothAddress
                            BuildVersion
                            DeviceCertificate
                            DeviceClass
                            DeviceName
                            DevicePublicKey
                            FirmwareVersion
                            HostAttached
                            IntegratedCircuitCardIdentity
                            InternationalMobileEquipmentIdentity
                            InternationalMobileSubscriberIdentity
                            ModelNumber
                            PhoneNumber
                            ProductType
                            ProductVersion
                            ProtocolVersion
                            RegionInfo
                            SBLockdownEverRegisteredKey
                            SIMStatus
                            SerialNumber
                            SomebodySetTimeZone
                            TimeIntervalSince1970
                            TimeZone
                            TimeZoneOffsetFromUTC
                            TrustedHostAttached
                            UniqueDeviceID
                            Uses24HourClock
                            WiFiAddress
                            iTunesHasConnected
                 */


        private void button1_Click(object sender, EventArgs e)
        {
            DeviceInfo device = CIphoneRead.GetDeviceInfo();
            if (!device.IsConnected)
            {
                MessageBox.Show("未连接");
                return;
            }
            string strShow = string.Format("设备名称：{0}\r\n设备标识：{1}\r\n产品类型：{2}\r\n系统版本：{3}\r\n序列11号：{4}\r\nIMEI：{5}\r\n激活状态：{6}\r\nICCID:{7}\r\n设备型号：{8}",
                device.DeviceName, device.DeviceId, device.DeviceType, device.DeviceVersion,
                device.DeviceSerial,
                device.IMEI,
                device.ActivationState,
                device.ICCID,
                device.ModelNumber);
            strShow = strShow.Replace('\0', ' ');
            MessageBox.Show(strShow);
        }
    }
}
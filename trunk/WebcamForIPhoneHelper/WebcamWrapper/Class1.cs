using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace WebcamWrapper
{
    public class WebcamWrapper
    {
        [DllImport("WebcamForIPhoneHelper.dll")]
        extern static int CapturePictrue(string strDefaultPath, string strPath);

        public static int DoCapturePictrue(string strDefaultPath, string strPath)
        {
            return CapturePictrue(strDefaultPath, strPath);
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Android.App;
using Android.Content;
using Android.OS;
using Android.Runtime;
using Android.Views;
using Android.Widget;
using System.Threading;
using AndroidApplication1;

namespace TragicGuy
{
    [Activity(Theme = "@style/Theme.Splash", MainLauncher = true, NoHistory = true, Icon = "@drawable/icon")]
    public class TheTheme : Activity
    {
        protected override void OnCreate(Bundle bundle)
        {
            base.OnCreate(bundle);

            Thread.Sleep(2000); // Simulate a long loading process on app startup.
            StartActivity(typeof(Activity1));
        }
    }
}
using System;

using Android.App;
using Android.Content;
using Android.Runtime;
using Android.Views;
using Android.Widget;
using Android.OS;
using TragicGuy;
using Android.Webkit;
using ClassLibrary;
using Android.Net;

namespace AndroidApplication1
{
    [Activity(Label = "MonoApk")]
    public class Activity1 : Activity
    {
        string[] items;
        protected override void OnCreate(Bundle bundle)
        {
            base.OnCreate(bundle);

            // Set our view from the "main" layout resource
            SetContentView(Resource.Layout.Main);

            // Get our button from the layout resource,
            // and attach an event to it    
            ListView listView1 = FindViewById<ListView>(Resource.Id.listView1);
            items = new string[] { "Fruits", "Flower Buds"};
            listView1.Adapter = new ArrayAdapter<String>(this, Android.Resource.Layout.SimpleListItem1, items);
            listView1.ItemClick += new EventHandler<ListView.ItemClickEventArgs>(OnListItemClick);
            Spinner spinner = FindViewById<Spinner>(Resource.Id.spinner1);
            spinner.ItemSelected += new EventHandler<AdapterView.ItemSelectedEventArgs>(spinner_ItemSelected);
            var adapter = ArrayAdapter.CreateFromResource(
                    this, Resource.Array.planets_array, Android.Resource.Layout.SimpleSpinnerItem);
             
            adapter.SetDropDownViewResource(Android.Resource.Layout.SimpleSpinnerDropDownItem);
            spinner.Adapter = adapter;

            Button mybutton1 = FindViewById<Button>(Resource.Id.MyButton1);

            mybutton1.Click += delegate
            {
                
                TextView textView1 = FindViewById<TextView>(Resource.Id.textView1);
                textView1.Text = "正在获取数据，请稍后......";
                ClassHttpPost webPost = new ClassHttpPost();
                webPost.GetHttpPageAsyc("http://www.web1800.com/web1800/hello.html", "utf-8", "GET", "", "", "", "*/*", "", response =>
                {
                    RunOnUiThread(() =>
                    {
                        textView1.Text = "网络访问返回结果：" + response;
                    });
                });
            };
            Button button1 = FindViewById<Button>(Resource.Id.button1);

            button1.Click += delegate
            {
                WebView webView = FindViewById<WebView>(Resource.Id.webView1);
                webView.LoadUrl(@"http://www.baidu.com");
            };
            
        }
        private void spinner_ItemSelected(object sender, AdapterView.ItemSelectedEventArgs e)
        {
            Spinner spinner = (Spinner)sender;            
            string toast = string.Format("您选择了下拉列表项： {0}", spinner.GetItemAtPosition(e.Position));
            Toast.MakeText(this, toast, ToastLength.Long).Show();
            TextView textView1 = FindViewById<TextView>(Resource.Id.textView1);
            textView1.Text = toast;
            
        }
        protected void OnListItemClick(object sender, AdapterView.ItemClickEventArgs e)
        {
            ListView listView1 = (ListView)sender;
            var t = items[e.Position];
            Android.Widget.Toast.MakeText(this, t, Android.Widget.ToastLength.Short).Show();
            TextView textView1 = FindViewById<TextView>(Resource.Id.textView1);
            textView1.Text = "您点击了列表项：" + t;
        }
     }
}


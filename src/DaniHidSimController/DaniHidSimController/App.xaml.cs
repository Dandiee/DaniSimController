using System;
using System.IO;
using System.Windows;
using DaniHidSimController.Models;
using DaniHidSimController.Mvvm;
using DaniHidSimController.Services;
using DaniHidSimController.ViewModels;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;

namespace DaniHidSimController
{
    public partial class App
    {
        public static IServiceProvider ServiceProvider { get; private set; }

        protected override void OnStartup(StartupEventArgs e)
        {
            var host = Host.CreateDefaultBuilder()
                .ConfigureAppConfiguration(configuration =>
                {
                    configuration
                        .SetBasePath(Directory.GetCurrentDirectory())
                        .AddJsonFile("appSettings.json")
                        .AddUserSecrets<App>();
                })
                .ConfigureServices((context, services) =>
                {
                    services.Configure<SimOptions>(context.Configuration.GetSection(nameof(SimOptions)));

                    services.AddTransient<MainWindow>();
                    services.AddTransient<MainWindowViewModel>();
                    services.AddTransient<LocationWindowViewModel>();
                    services.AddTransient<DeviceViewModel>();
                    services.AddTransient<DeviceViewModel>();

                    services.AddSingleton<IHidService, HidService>();
                    services.AddSingleton<IUsbService, UsbService>();
                    services.AddSingleton<IEventAggregator, EventAggregator>();
                    services.AddSingleton<ISimConnectService, SimConnectService>();
                })
                .Build();

            ServiceProvider = host.Services;

            host.Services.GetService<MainWindow>().Show();
        }
    }
}

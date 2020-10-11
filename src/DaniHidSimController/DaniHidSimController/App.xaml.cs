using System;
using System.IO;
using System.Windows;
using DaniHidSimController.Models;
using DaniHidSimController.Mvvm;
using DaniHidSimController.Services;
using DaniHidSimController.ViewModels;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;

namespace DaniHidSimController
{
    public partial class App
    {
        public static IServiceProvider ServiceProvider { get; private set; }
        public static IConfiguration Configuration { get; private set; }

        protected override void OnStartup(StartupEventArgs e)
        {
            var serviceCollection = new ServiceCollection();

            var configurationBuilder = new ConfigurationBuilder()
                .SetBasePath(Directory.GetCurrentDirectory())
                .AddJsonFile("appSettings.json", optional: true, reloadOnChange: true)
                .AddUserSecrets<App>();

            Configuration = configurationBuilder.Build();

            ConfigureServices(serviceCollection, Configuration);

            ServiceProvider = serviceCollection.BuildServiceProvider();

            var mainWindow = ServiceProvider.GetRequiredService<MainWindow>();
            mainWindow.Show();
        }

        private void ConfigureServices(IServiceCollection services, IConfiguration configuration)
        {
            services.AddSingleton<IEventAggregator, EventAggregator>();
            services.AddTransient<MainWindow>();
            services.Configure<SimOptions>(configuration.GetSection(nameof(SimOptions)));
            services.AddTransient<MainWindowViewModel>();
            services.AddTransient<LocationWindowViewModel>();
            services.AddTransient<DeviceViewModel>();

            services.AddSingleton<IHidService, HidService>();
            services.AddSingleton<IUsbService, UsbService>();
            services.AddTransient<DeviceViewModel>();
            services.AddSingleton<ISimConnectService, SimConnectService>();
        }
    }
}

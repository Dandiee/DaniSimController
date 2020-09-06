using System;
using System.Windows;
using DaniSimController.Mvvm;
using DaniSimController.Services;
using DaniSimController.ViewModels;
using DaniSimController.Views;
using Microsoft.Extensions.DependencyInjection;

namespace DaniSimController
{
    public partial class App
    {
        public static IServiceProvider ServiceProvider { get; private set; }


        protected override void OnStartup(StartupEventArgs e)
        {
            var serviceCollection = new ServiceCollection();
            ConfigureServices(serviceCollection);

            ServiceProvider = serviceCollection.BuildServiceProvider();

            var mainWindow = ServiceProvider.GetRequiredService<MainWindow>();
            mainWindow.Show();
        }

        

        private void ConfigureServices(IServiceCollection services)
        {
            services.AddSingleton<IEventAggregator, EventAggregator>();
            services.AddSingleton<ISimVarComponent, SimVarComponent>();
            services.AddSingleton<IPoller, Poller>();
            
            services.AddTransient(typeof(MainWindow));

            services.AddTransient(typeof(MainWindowViewModel));
            services.AddTransient(typeof(SimVarBrowserViewModel));
            services.AddTransient(typeof(SimVarMonitorViewModel));
            services.AddTransient(typeof(PollingViewModel));
            services.AddTransient(typeof(SerialViewModel));
        }

    }
}

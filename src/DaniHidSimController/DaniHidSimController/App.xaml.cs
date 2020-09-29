using System;
using System.Windows;
using DaniHidSimController.Mvvm;
using DaniHidSimController.Services;
using DaniHidSimController.ViewModels;
using Microsoft.Extensions.DependencyInjection;

namespace DaniHidSimController
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
            services.AddTransient<MainWindow>();
            services.AddTransient<MainWindowViewModel>();
            services.AddTransient<IHidService, HidService>();
        }
    }
}

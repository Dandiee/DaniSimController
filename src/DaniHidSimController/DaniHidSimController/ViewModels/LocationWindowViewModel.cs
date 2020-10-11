using System;
using System.Diagnostics;
using System.Windows.Input;
using DaniHidSimController.Models;
using DaniHidSimController.Mvvm;
using DaniHidSimController.Services;
using Microsoft.Extensions.Options;
using Microsoft.Maps.MapControl.WPF;
using Microsoft.Maps.MapControl.WPF.Core;

namespace DaniHidSimController.ViewModels
{
    public sealed class LocationWindowViewModel : BindableBase
    {
        private readonly TimeSpan _refreshCooldown;

        public ICommand CheckLocationCommand { get; }

        public CredentialsProvider CredentialsProvider { get; }

        public LocationWindowViewModel(
            IEventAggregator eventAggregator,
            IOptions<SimOptions> options)
        {
            _refreshCooldown = TimeSpan.FromMilliseconds(options.Value.BindMapLocationUpdateCooldownInMs);
            _location = new Location(47.493351, 19.060372);
            CredentialsProvider = new ApplicationIdCredentialsProvider(options.Value.BingMapCredentialsProvider);
            CheckLocationCommand = new DelegateCommand(CheckLocation);
            eventAggregator.GetEvent<SimVarReceivedEvent>().Subscribe(OnSimVarReceived);
        }

        private DateTime _lastLocationUpdate;

        private Location _location;
        public Location Location
        {
            get => _location;
            set
            {
                if (DateTime.Now - _lastLocationUpdate > _refreshCooldown)
                {
                    SetProperty(ref _location, value);
                    _lastLocationUpdate = DateTime.Now;
                }
                else
                {
                    _location = value;
                }
            }
        }

        private void OnSimVarReceived(SimVarRequest request)
        {
            if (request.SimVar == SimVars.GPS_POSITION_LAT)
            {
                Location = new Location((float)request.Get() * (180 / Math.PI), Location.Longitude);
            }
            else if(request.SimVar == SimVars.GPS_POSITION_LON)
            {
                Location = new Location(Location.Latitude, (float)request.Get() * (180 / Math.PI));
            }
        }

        private void CheckLocation()
            => Process.Start(
                new ProcessStartInfo($"https://www.google.com/maps/search/{Location.Latitude}+{Location.Longitude}")
                    {UseShellExecute = true});
    }
}

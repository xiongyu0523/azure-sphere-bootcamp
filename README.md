# Azure-Sphere-Bootcamp

This repo is built for Azure Sphere Bootcamp Hands-on day. It provides all required information for trainee to follow and finish all hands-on labs. 

# Pre-Lab prerequisites

Follow [Install Azure Sphere](https://docs.microsoft.com/en-us/azure-sphere/install/overview) section to complete all neccessary steps before start. 

Quick check list:
- FTDI driver is installed and three COMx ports are present in Device Manager 
- Up to date Visual Studio and Azure Sphere SDK are installed
- Has registered a Azure account and a free subscription or pay-as-you-go subscription
- Has logged in Azure Sphere command line utility by `azsphere login`
- Create a Azure Sphere tenant by `azsphere tenant create -n <tenant name>` if there is no tenant in your orgnization.
- Has selected Azure Sphere tenant by `azsphere tenant select -i <tenant id>`
- (New device ONLY) Device is claimed to user's tenant by `azsphere device claim`
- Device is recovered by `azsphere device recover` command to have a knowning good Azure Sphere OS.
- [Git](https://git-scm.com/download/win) is installed and added to PATH.

# Labs

- [Lab-1 Blinking LED](Lab-1.md)
- [Lab-2 Application Over-the-Air deployment](Lab-2.md)
- [Lab-3 Connect to Azure IoT Hub](Lab-3.md)
- [Lab-4 Visualize real world data on Azure IoT Central](Lab-4.md)
# Azure-Sphere-Bootcamp

This repo is for Azure Sphere Bootcamp Hands-on day. 

# Pre-Lab prerequisites

Follow [Install Azure Sphere](https://docs.microsoft.com/en-us/azure-sphere/install/overview) section to complete all neccessary steps before we start labs. 

Quick check list:
- FTDI driver is installed and three COMx port are in Device Manager 
- Visual Studio and Azure Sphere SDK are installed
- User has registered a Azure account and a free subscription or pay-as-you-go subscription
- User has logged in Azure Sphere utility by `azsphere login`
- User has selected Azure Sphere tenant by `azsphere tenant select -i <tenant id>`
- (New device ONLY) Device is claimed to user's tenant by `azsphere device claim`
- To recover the device by `azsphere device recover`



# Lab-1: Blink LED

## Goals

- Understand the basic sketch of Azure Sphere High-level Applicaiton
- Leann how to build & debug Application using Visual Studio
- Learn how to use GPIO API from Azure Sphere SDK

## Steps

1. Enable device debug and disable OTA in Azure Sphere utility
   
   `azsphere device prep-debug`

2. Clone Azure Sphere Samples repo

    `git clone https://github.com/Azure/azure-sphere-samples`

3. Open a project or solution in Visual Studio and navigate to the folder of *.\azure-sphere-samples\Samples\GPIO\GPIO_HighLevelApp*, open GPIO_HighLevelApp.sln and press **F5** to build and load the application onto the device for debugging.
   
 ![](images/open-project.png)
   
4. LED1 start to blink after Application is loaded. Press **Button A** can adjust the blinking rate. Both LED1 and Button A are connected to GPIO pin and controlled by the high-level Application.

5. Select Line 74 and press **F9** to set a breakpoint on function *GPIO_SetValue*, you will see the program is halt and stop on this line. Continue with **F5** and now you can control the ON/OFF state of LED.

 ![](images/debug.png)

## Read more
- [Using GPIOs on Azure Sphere](https://docs.microsoft.com/en-us/azure-sphere/app-development/gpio)
- [Best practices for high-level Application](https://docs.microsoft.com/en-us/azure-sphere/app-development/initialization-termination)
- [Manage target hardware dependencies](https://docs.microsoft.com/en-us/azure-sphere/app-development/manage-hardware-dependencies)
- [Understand Epoll and Timefd]()

# Lab-2: Over-the-Air upgrade 

## Goals

- Learn how to configure WiFi network for Azure Sphere
- Understand all the concepts in Over-The-Air deployment
- Learn how to use Azure Sphere utiliy to deploy an Application Over-the-Air
  
## Steps 

1. Configure the WiFi credentials and allow Azure Sphere to connect Azure Sphere Security Service in the background. Use command:
   
   `azsphere device wifi add --ssid <yourSSID> --key <yourNetworkKey>`
   
   > - To add an open network, omit the --key flag
   > - If your network SSID or key has embedded spaces, enclose in quotation marks, e.g. --ssid "My iPhone"

2. Check WiFi status and confirm it is connnected to the AP.
   
   `azsphere device wifi show-status`

    ![](images/show-wifi-status.png)
    
3. Create a *product SKU* and assign an unique name, record the GUID for later use. The description parameter is optional. 

   `azsphere sku create --name <sku-name> --description <optional-desc>`
   
4. Create a *device group* and assign an unique name, record the GUID for later use. This device group has a default policy that will accept all updates from the Azure Sphere Security Service (Both OS and App)

    `azsphere device-group create --name <device-group-name>`

5. Attach SKU to the device and assign the device into the device group. In the meantime, the following command delete existing applications, disable debug capability. Now device only accept production-siged image from Azure Sphere Security Service (aka AS3). 

    `azsphere device prep-field --skuid <productsku-GUID> --devicegroupid <devgroup-GUID>`

    At this point, you will see previous loaded LED blink application is not working. 

6. Running command `azsphere device show-ota-config` will see the device related information on AS3, record the *chip SKU* for later use.

   ![](images/chip-sku.png)

7. Upload your application to AS3 and automatically create the component that represent your application:
   
   `azsphere component image add --autocreatecomponent --filepath <file_path>`

   The component ID and name are retreived from the *app_manifest.json* file, since the imagepackage already have this metadata. You can also specifiy a name and a GUID for a component using `azsphere component create`, just make sure it should be consistent with the value in manifest.

    ![](images/component-id.png)

   Write down the image ID for next-step use. This ID represent your current version of application, it changes for each build. 

    ![](images/image-id.png)

8. Create a image set link to the image uploaded to AS3. Fill the image GUID found in previous step. The image set name must be unique in current tenant.

    `azsphere image-set create --name <imageset-name> --imageid <image-GUID>`

    > It is a good practice to use current time and data as ending of image-set name, such as ImageSet-GPIOHighLevelApp-2019.08.27.12.00

    Record the image-set GUID for Step. 10 use.
    ![](images/image-set.png)

9. Create an application feed target to the SKU set (chip SKU + product SKU) and component identified in prevous steps. 

    `azsphere feed create --name <feed-name> --componentid <component-id> --chipSkuid <chipsku-GUID> --productskuid <productsku-GUID> --dependentfeedid 3369f0e1-dedf-49ec-a602-2aa98669fd61`

    parameter --dependentfeedid specifies the ID of the Azure Sphere OS feed (3369f0e1-dedf-49ec-a602-2aa98669fd61). All application feed must have this dependence.

    Write down the feed GUID for the last two steps.

    ![](images/feed-id.png)

10. Add the feed to the device group.
    
    `azsphere device-group feed add --devicegroupid <devgroup-GUID> --feedid <feed-GUID>`

11. Add the image set to feed to active a deployment. 

    `azsphere feed image-set add --feedid <feed-GUID> --imagesetid <imageset-GUID>`

12. Reset the board and wait few minitues, LED1 will start to blink once depolyment is finished.

13. Once a feed is active, deploy a new image is pretty straightforward. Try to modify your application and run below command, see what happens to your board!

    `azsphere component publish --feedid <feed-GUID> --imagepath <file-path>`

## Read more
- [Azure Sphere OS networking requirements](https://docs.microsoft.com/en-us/azure-sphere/network/ports-protocols-domains)
- [Deloyment basics](https://docs.microsoft.com/en-us/azure-sphere/deployment/deployment-concepts)
- [Link the device to a feed](https://docs.microsoft.com/en-us/azure-sphere/deployment/link-to-feed)
- [Set up a device group for OS evaluation](https://docs.microsoft.com/en-us/azure-sphere/deployment/set-up-evaluation-device-group)

# Lab-3: Connect to Azure IoT Hub

# Lab-4: Connect to Azure IoT Central 


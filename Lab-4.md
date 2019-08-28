# Lab-4: Visualize real world data on Azure IoT Central

- Go to [Home Page](README.md)

## Goals

- Hands-on with Azure IoT Central configuration
- Understand the basic feature of IoT Central offered as a IoT SaaS solution
- Get familiar with I2C API to connect to external sensor device
- Use Azure Sphere and Azure IoT Central work together to build a secured end-to-end solution with telemtery collection, visulization and remote control ability.

## Steps

1. After connect Azure Sphere development board to your PC, enable device debug and disable OTA in Azure Sphere utility by:
   
   `azsphere device prep-debug`

2. Make sure WiFi credential is configured and Azure Sphere is connected to AP. If Lab-2 and Lab-3 are skipped, follow Step 1) and 2) in [Ove-The-Air upgrade](Lab-2.md) to setup.
   
3. Follow [Setup IoT Central](https://docs.microsoft.com/en-us/azure-sphere/app-development/setup-iot-central) page to setup cloud resources, the key steps are:
    
    - Create Azure IoT Central Application
    - Upload tenant CA certificate to IoT Central and finish Proof of Procession

4. Go to your application in [Azure IoT Central](https://apps.azureiotcentral.com/) and click **Create Device Templates** on the home page, select **Custom** to start a blank template. Give a name to your template and click **Create** button. 

    ![](images/AzureSphereTemplate.png)
   
5. Click **+ New** button and select **Telemetry**
   
   1. Set the Display Name and Field Name to **Temperature**. The Field Name must exactly match the name in the sample code, so this value is case-sensitive. Set Units to Degrees, and set the Minimum Value and Maximum Value to -40 and 85, respectively. Then click Save.

   2. Set one more telemetry **Humidity**, set range and unit to 0% to 100%
   3. Set one more telemetry **Pressure**, set range and unit to 300 to 1100 hPa

6. Click **+ New** button and select **Event**, Set the Display Name and Field Name to **ButtonPress**. Then click Save. 

    ![](images/template.png)

7. On the left Side Navigation select **Devices**, click + to add a new device, and select **Real** from the drop-down menu. A dialog will prompt to ask for Device ID and Name.

    ![](images/Realdevice.png)

8. In an Azure Sphere Developer Command Prompt, type the following command:
   
   `powershell -Command ((azsphere device show-attached)[0] -split ': ')[1].ToLower()`

   Copy the lowercase device ID and paste it into the Device ID field of the Create New Device dialog box. Device name will be changed automatically using the device ID appended. Click **Create** button.

9. Open a solution in Visual Studio and navigate to the folder of 0*.\azure-sphere-samples\Samples\AzureIoT*, open AzureIoT.sln solution file. Before build the project, there're three key information you need provide to the applicaton in app_manifest.json file.
   
   - The Tenant ID for your Azure Sphere device
   - The Scope ID for your Azure IoT Central application
   - The IoT hub URL for your Azure IoT Central application 

    Tenant ID can be retrieve from Azure Sphere command line utility by
    `azsphere tenant show-selected`

    ![](images/tenant.png)

    To get Scope ID and IoT Hub URL, there is a assist tool in *.\azure-sphere-samples\Samples\AzureIoT\Tools* folder can help. In Azure Sphere command line utility, goo to above location, and run `ShowIoTCentralConfig.exe`, input 'Y' and login with your credential if needed. 

    ![](images/ShowIoTCentralConfig.png)

    Fill these information in app_manifest.json file as below:

    ![](images/central_manifest.png)

10. Connect BME280 sensor board to I2C pin of ISU2 on Azure Sphere Development board. They're 4 pins: VCC, GND, SDA and SCL. The CSB and SDO pin on sensor board can be left unconnected.

    ![](images/MT3620_DVB_I2C.png)

11. Grant ISU2 I2C access permission to applicaiton in app_manifest.json. The ISU2 I2C is described as **SAMPLE_LSM6DS3_i2C** in sample_hardware.json hardware definition file. We just use this name for simplicity.
    
    ![](images/i2c.png)

12. By default the Azure IoT sample project send simulated temperature sensor data to Azure IoT. Hack the code to visualize your real world data!

    > BME280 driver is provided in Code/Lab-4 folder. There're only two API exposed in bme280_uesr.h will be needed by your application. 
    
    ```
    int bme280_component_config (int isu_port);
    int bme280_component_getdata(struct bme280_data* pData);
    ```

13. Since I2C is still a [BETA](https://docs.microsoft.com/en-us/azure-sphere/app-development/use-beta) feature in current Azure Sphere SDK. We need select **2+Beta1905** as target API set in **Project->Azure IoT Property Pages** before build the project.
    
    ![](images/i2cabi.png)

14. Press F5 to build and run the applicaiton. Go to the device dashboard to check your data.

    ![](images/datapreview.png)

15. Follow this [page](https://github.com/xiongyu0523/azure-sphere-samples/blob/master/Samples/AzureIoT/IoTCentral.md#add-new-measurements-settings-and-properties) to configure IoT Central application to visulize a state `Orientation` report from device and use toggle setting to control LED1.

## Read more

- [What is Azure IoT Central](https://docs.microsoft.com/en-us/azure/iot-central/overview-iot-central)
- [Azure IoT Central Architecture](https://docs.microsoft.com/en-us/azure/iot-central/concepts-architecture)
- [Use I2C with Azure Sphere](https://docs.microsoft.com/en-us/azure-sphere/app-development/i2c)
- [Manage target hardware dependencies](https://docs.microsoft.com/en-us/azure-sphere/app-development/manage-hardware-dependencies)



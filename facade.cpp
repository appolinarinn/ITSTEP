#include <iostream>
#include <string>
using namespace std;

class GPU
{
public:
	void start()
	{
		cout << "GPU started\n";
	}
	void checkMonitorConnection()
	{
		cout << "Monitor connected\n";
	}
	void displayRAMInfo()
	{
		cout << "RAM info displayed\n";
	}
	void displayOpticalDriveInfo()
	{
		cout << "Optical drive info displayed\n";
	}
	void displayHDDInfo()
	{
		cout << "Hard drive info displayed\n";
	}
	void stop()
	{
		cout << "GPU stopped\n";
	}

};

class RAM
{
public:
	void start()
	{
		cout << "RAM started\n";
	}
	void analyzeMemory()
	{
		cout << "Memory analyzed\n";
	}
	void stop()
	{
		cout << "RAM stopped\n";
	}
};

class HDD
{
public:
	void start()
	{
		cout << "HDD started\n";
	}
	void checkBootSector()
	{
		cout << "Boot sector checked\n";
	}
	void stop()
	{
		cout << "HDD stopped\n";
	}
};

class OpticalDrive
{
public:
	void start()
	{
		cout << "Optical drive started\n";
	}
	void checkDisk()
	{
		cout << "Disk checked\n";
	}
	void stop()
	{
		cout << "Optical drive stopped\n";
	}
};

class Power
{
public:
	void supplyPower()
	{
		cout << "Power supplied\n";
	}
	void powerToGPU()
	{
		cout << "Power supplied to graphics card\n";
	}
	void powerToRAM()
	{
		cout << "Power supplied to RAM\n";
	}
	void powerToHDD()
	{
		cout << "Power supplied to HDD\n";
	}
	void powerToOpticalDrive()
	{
		cout << "Power supplied to optical drive\n";
	}
	void cutPowerToGPU()
	{
		cout << "Power cut to graphics card\n";
	}
	void cutPowerToRAM()
	{
		cout << "Power cut to RAM\n";
	}
	void cutPowerToHDD()
	{
		cout << "Power cut to HDD\n";
	}
	void cutPowerToOpticalDrive()
	{
		cout << "Power cut to optical drive\n";
	}
	void shutDown()
	{
		cout << "Power supply stopped\n";
	}
};

class Sensor
{
public:
	void checkVoltage()
	{
		cout << "Voltage checked\n";
	}
	void checkPowerTemperature()
	{
		cout << "Power supply temperature checked\n";
	}
	void checkGPUTemperature()
	{
		cout << "Graphics card temperature checked\n";
	}
	void checkRAMTemperature()
	{
		cout << "RAM temperature checked\n";
	}
	void checkAllSystemsTemperature()
	{
		cout << "All systems temperature checked\n";
	}
	void stop()
	{
		cout << "Sensor stopped\n";
	}
};


class PCFacade
{
	GPU gpu;
	RAM ram;
	HDD hdd;
	OpticalDrive opticalDrive;
	Power power;
	Sensor sensor;
public:
	PCFacade(GPU gpu, RAM ram, HDD hdd, OpticalDrive opticalDrive, Power power, Sensor sensor)
	{
		this->gpu = gpu;
		this->ram = ram;
		this->hdd = hdd;
		this->opticalDrive = opticalDrive;
		this->power = power;
		this->sensor = sensor;
	}
	void BeginWork()
	{
		power.supplyPower();
		sensor.checkVoltage();
		sensor.checkPowerTemperature();
		sensor.checkGPUTemperature();
		power.powerToGPU();
		gpu.start();
		gpu.checkMonitorConnection();
		sensor.checkRAMTemperature();
		power.powerToRAM();
		ram.start();
		ram.analyzeMemory();
		gpu.displayRAMInfo();
		power.powerToOpticalDrive();
		opticalDrive.start();
		opticalDrive.checkDisk();
		gpu.displayOpticalDriveInfo();
		power.powerToHDD();
		hdd.start();
		hdd.checkBootSector();
		gpu.displayHDDInfo();
		sensor.checkAllSystemsTemperature();
	}
	void EndWork()
	{
		hdd.stop();
		ram.stop();
		ram.analyzeMemory();
		gpu.stop();
		opticalDrive.stop();
		power.cutPowerToGPU();
		power.cutPowerToRAM();
		power.cutPowerToOpticalDrive();
		power.cutPowerToHDD();
		sensor.checkVoltage();
		power.shutDown();
	}
};

class Client
{
public:
	void CreatePC(PCFacade facade)
	{
		facade.BeginWork();
		facade.EndWork();
	}
};

int main()
{
	GPU gpu;
	RAM ram;
	HDD hdd;
	OpticalDrive opticalDrive;
	Power power;
	Sensor sensor;

	PCFacade pc(gpu, ram, hdd, opticalDrive, power, sensor);

	Client client;
	client.CreatePC(pc);

	system("pause");
	return 0;
}
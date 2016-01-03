package dav795.vm;

public interface CentralProcessingUnit
{
	void initialize();
	void addMMU(MemoryManagementUnit mmu);
	void addPeripheral(int id, Peripheral p);
	int getPeripheralCapacity();
	void 
}

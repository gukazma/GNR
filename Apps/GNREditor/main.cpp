#include <NRIFramework.h>
static const char *vendors[] = {
    "unknown",
    "NVIDIA",
    "AMD",
    "INTEL",
};

static const char *architectures[] = {
    "unknown",
    "INTEGRATED",
    "DESCRETE",
};
class GNR : public SampleBase
{
public:
	GNR() : SampleBase() {}
    ~GNR();

    // 通过 SampleBase 继承
    bool Initialize(nri::GraphicsAPI graphicsAPI, bool) override
    {
        // 先查询有多少个适配器
        uint32_t adaptersNum = 0;
        NRI_ABORT_ON_FAILURE(nri::nriEnumerateAdapters(nullptr, adaptersNum)); // 传 nullptr 查询数量
        // Initialize NRI and other components
        std::vector<nri::AdapterDesc> adapterDescs(adaptersNum);
        NRI_ABORT_ON_FAILURE(nri::nriEnumerateAdapters(adapterDescs.data(), adaptersNum)); // 传 nullptr 查询数量
        printf("nriEnumerateAdapters: %u adapters reported\n", adaptersNum);
        for (uint32_t i = 0; i < adaptersNum; i++)
        {
            const nri::AdapterDesc adapterDesc = adapterDescs[i];

            printf("\nAdapter #%u\n", i + 1);
            printf("\tName                 : %s\n", adapterDesc.name);
            printf("\tVendor               : %s\n", vendors[int(adapterDesc.vendor)]);
            printf("\tArchitecture         : %s\n", architectures[int(adapterDesc.architecture)]);
            printf("\tVideo memory         : %" PRIu64 " Mb\n", adapterDesc.videoMemorySize >> 20);
            printf("\tShared system memory : %" PRIu64 " Mb\n", adapterDesc.sharedSystemMemorySize >> 20);
            printf("\tQueues               : {%u, %u, %u}\n", adapterDesc.queueNum[0], adapterDesc.queueNum[1],
                   adapterDesc.queueNum[2]);
            printf("\tID                   : 0x%08X\n", adapterDesc.deviceId);
            printf("\tLUID                 : 0x%016" PRIX64 "\n", adapterDesc.luid);
        }

        return true;
    }
    void RenderFrame(uint32_t frameIndex) override
    {

    }
};


GNR::~GNR() {
	// Cleanup code if needed
}


SAMPLE_MAIN(GNR, 0);
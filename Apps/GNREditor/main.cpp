#include <NRIFramework.h>
#include <spdlog/spdlog.h>
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

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
        SPDLOG_INFO("nriEnumerateAdapters: {} adapters reported", adaptersNum);
        for (uint32_t i = 0; i < adaptersNum; i++)
        {
            const nri::AdapterDesc adapterDesc = adapterDescs[i];

            SPDLOG_INFO("Adapter #{}:", i + 1);
            SPDLOG_INFO("\tName                 : {}", adapterDesc.name);
            SPDLOG_INFO("\tVendor               : {}", vendors[int(adapterDesc.vendor)]);
            SPDLOG_INFO("\tArchitecture         : {}", architectures[int(adapterDesc.architecture)]);
            SPDLOG_INFO("\tVideo memory         : {} Mb", adapterDesc.videoMemorySize >> 20);
            SPDLOG_INFO("\tShared system memory : {} Mb", adapterDesc.sharedSystemMemorySize >> 20);
            SPDLOG_INFO("\tQueues               : {{ {}, {}, {} }}", adapterDesc.queueNum[0], adapterDesc.queueNum[1],
                        adapterDesc.queueNum[2]);
            SPDLOG_INFO("\tID                   : 0x{:08X}", adapterDesc.deviceId);
            SPDLOG_INFO("\tLUID                 : 0x{:016X}", adapterDesc.luid);
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
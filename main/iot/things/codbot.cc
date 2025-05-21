#include "iot/thing.h"
#include "board.h"
#include "audio_codec.h"
#include "protocol.h"
#include "mqtt_protocol.h"

#include <driver/gpio.h>
#include <esp_log.h>

#define TAG "Codbot"

namespace iot {

class Codbot : public Thing {
private:
    std::unique_ptr<MqttProtocol> cusmqttprotocol = std::make_unique<MqttProtocol>();
public:
    Codbot() : Thing("Robot", "你是一只机器狗，可以执行一些简单的动作和识别内容") {
        // 定义设备的属性
        // properties_.AddBooleanProperty("power", "灯是否打开", [this]() -> bool {
            // return power_;
        // });

        // 定义设备可以被远程执行的指令
        methods_.AddMethod("MoveForward", "向前移动", ParameterList(), [this](const ParameterList& parameters) {
            ESP_LOGI(TAG, "向前移动");
            cusmqttprotocol->StartCusMqttClient();
            cusmqttprotocol->CustomPublish("move_forward", "1");
        });

        methods_.AddMethod("MoveBackward", "向后移动", ParameterList(), [this](const ParameterList& parameters){
            ESP_LOGI(TAG, "向后移动");
            cusmqttprotocol->StartCusMqttClient();
            cusmqttprotocol->CustomPublish("move_backward", "1");
        })

        methods_.AddMethod("TurnLeft", "向左转", ParameterList(), [this](const ParameterList& parameters) {
            ESP_LOGI(TAG, "向左转");
            cusmqttprotocol->StartCusMqttClient();
            cusmqttprotocol->CustomPublish("turn_left", "1");
        });

        methods_.AddMethod("TurnRight", "向右转", ParameterList(), [this](const ParameterList& parameters) {
            ESP_LOGI(TAG, "向右转");
            cusmqttprotocol->StartCusMqttClient();
            cusmqttprotocol->CustomPublish("turn_right", "1");
        });
        
        methods_.AddMethod("OpenCamera", "识别内容分析", ParameterList({
            Parameter("mode", "用户提出的完整问题，保证不删减", kValueTypeString, true)
        }), [this](const ParameterList& parameters) {
            std::string mode = parameters["mode"].string();
            ESP_LOGI(TAG, "摄像头识别请求，内容: %s", mode.c_str());
            cusmqttprotocol->StartCusMqttClient();
            cusmqttprotocol->CustomPublish("open_camera", mode);
        });



    }
};

} // namespace iot

DECLARE_THING(Codbot);

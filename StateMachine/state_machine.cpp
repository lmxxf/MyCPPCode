#include <iostream>
#include <memory>
#include <string>

// 定义输入事件
enum class Event { Timer };

// 状态机基类
class StateMachine {
public:
    virtual ~StateMachine() = default;
    virtual void handle_event(Event event) = 0;
};

// 交通信号灯状态基类
class TrafficLightState : public StateMachine {
public:
    virtual std::string get_state_name() const = 0;
};

// 前向声明状态类，以便在TrafficLight中使用
class RedState;
class YellowState;
class GreenState;

// 交通信号灯类
class TrafficLight {
public:
    TrafficLight();
    void set_state(std::unique_ptr<TrafficLightState> new_state);
    void handle_event(Event event);
    std::string get_state_name() const;

private:
    std::unique_ptr<TrafficLightState> current_state_;
};

// 红灯状态类
class RedState : public TrafficLightState {
public:
    RedState(TrafficLight* light) : light_(light) {}

    void handle_event(Event event) override {
        if (event == Event::Timer) {
            std::cout << "Switching from Red to Green." << std::endl;
            light_->set_state(std::unique_ptr<TrafficLightState>(new GreenState(light_)));
        }
    }

    std::string get_state_name() const override {
        return "Red";
    }

private:
    TrafficLight* light_;
};

// 黄灯状态类
class YellowState : public TrafficLightState {
public:
    YellowState(TrafficLight* light) : light_(light) {}

    void handle_event(Event event) override {
        if (event == Event::Timer) {
            std::cout << "Switching from Yellow to Red." << std::endl;
            light_->set_state(std::unique_ptr<TrafficLightState>(new RedState(light_)));
        }
    }

    std::string get_state_name() const override {
        return "Yellow";
    }

private:
    TrafficLight* light_;
};

// 绿灯状态类
class GreenState : public TrafficLightState {
public:
    GreenState(TrafficLight* light) : light_(light) {}

    void handle_event(Event event) override {
        if (event == Event::Timer) {
            std::cout << "Switching from Green to Yellow." << std::endl;
            light_->set_state(std::unique_ptr<TrafficLightState>(new YellowState(light_)));
        }
    }

    std::string get_state_name() const override {
        return "Green";
    }

private:
    TrafficLight* light_;
};


TrafficLight::TrafficLight() : current_state_(std::make_unique<RedState>(this)) {}

void TrafficLight::set_state(std::unique_ptr<TrafficLightState> new_state) {
    current_state_ = std::move(new_state);
}

void TrafficLight::handle_event(Event event) {
    current_state_->handle_event(event);
}

std::string TrafficLight::get_state_name() const {
    return current_state_->get_state_name();
}

int main() {
    TrafficLight light;
    
    std::cout << "Current state: " << light.get_state_name() << std::endl;
    light.handle_event(Event::Timer);
    std::cout << "Current state: " << light.get_state_name() << std::endl;
    light.handle_event(Event::Timer);
    std::cout << "Current state: " << light.get_state_name() << std::endl;
    light.handle_event(Event::Timer);
    std::cout << "Current state: " << light.get_state_name() << std::endl;
    light.handle_event(Event::Timer);
    std::cout << "Current state: " << light.get_state_name() << std::endl;
    light.handle_event(Event::Timer);
    std::cout << "Current state: " << light.get_state_name() << std::endl;

    return 0;
}


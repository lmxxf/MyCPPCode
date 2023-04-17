#include <iostream>
#include <pjsua2.hpp>

using namespace pj;

class MyAccount : public Account
{
public:
    virtual void onRegState(OnRegStateParam &prm)
    {
        AccountInfo ai = getInfo();
        std::cout << (ai.regIsActive ? "*** Register: code=" : "*** Unregister: code=") << prm.code << std::endl;
    }
};

class MyCall : public Call
{
public:
    virtual void onCallState(OnCallStateParam &prm)
    {
        CallInfo ci = getInfo();
        std::cout << "*** Call: " << ci.remoteUri << " [" << ci.stateText << "]" << std::endl;

        if (ci.state == PjsipInvState::PJSIP_INV_STATE_DISCONNECTED) {
            std::cout << "*** Hangup" << std::endl;
            delete this;
        }
    }
};

int main(int argc, char *argv[])
{
    Endpoint ep;

    try {
        ep.libCreate();

        // 初始化库
        EpConfig ep_cfg;
        ep.libInit(ep_cfg);

        // 创建 UDP 传输
        TransportConfig transport_cfg;
        transport_cfg.port = 5060;
        ep.transportCreate(PJSIP_TRANSPORT_UDP, transport_cfg);

        // 启动库
        ep.libStart();

        // 创建账号
        AccountConfig acc_cfg;
        acc_cfg.idUri = "sip:test@127.0.0.1";
        acc_cfg.regConfig.registrarUri = "sip:127.0.0.1";
        acc_cfg.sipConfig.authCreds.push_back(AuthCredInfo("digest", "*", "test", 0, "test"));
        MyAccount *acc = new MyAccount;
        acc->create(acc_cfg);

        // 注册账号
        std::cout << "*** Register: " << acc_cfg.idUri << std::endl;
        acc->registerAcc();

        // 等待注册成功
        std::cout << "*** Wait for registration" << std::endl;
        pj_thread_sleep(2000);

        // 创建呼叫
        CallOpParam call_param;
        MyCall *call = new MyCall(*acc);
        call->makeCall("sip:test2@127.0.0.1", call_param);

        // 等待呼叫接通
        std::cout << "*** Wait for call to be answered" << std::endl;
        pj_thread_sleep(2000);

        // 挂断呼叫
        call->hangup();

        // 注销账号
        std::cout << "*** Unregister: " << acc_cfg.idUri << std::endl;
        acc->unreg();
        delete acc;

        // 关闭库
        ep.libDestroy();

        return 0;
    } catch (Error &err) {
        std::cerr << "Error: " << err.info() << std::endl;
        return 1;
    }
}

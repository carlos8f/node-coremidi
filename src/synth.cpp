#include <node.h>
#include <v8.h>
#include <nan.h>
#include <AudioUnit/AudioUnit.h>
#include <AudioToolbox/AudioToolbox.h>

using namespace node;

class Synth : public Nan::ObjectWrap
{
private:
    AUGraph graph;
    AudioUnit synthUnit;
public:
    static Nan::Persistent<v8::FunctionTemplate> s_ct;
    static void Init(v8::Handle<v8::Object> target)
    {
        Nan::HandleScope scope;
        v8::Local<v8::FunctionTemplate> t = Nan::New<v8::FunctionTemplate>(Synth::New);
        
        Nan::Persistent<v8::FunctionTemplate>(s_ct);
        t->InstanceTemplate()->SetInternalFieldCount(1);
        t->SetClassName(Nan::New<v8::String>("Synth").ToLocalChecked());
        
        Nan::SetPrototypeMethod(t, "send", Send);
        
        target->Set(Nan::New<v8::String>("synth").ToLocalChecked(), t->GetFunction());
    }
    
    Synth()
    {
        AUNode synthNode, outputNode;
        graph = 0;
        NewAUGraph(&graph);
        
        AudioComponentDescription cd;
        cd.componentManufacturer = kAudioUnitManufacturer_Apple;
        cd.componentFlags = 0;
        cd.componentFlagsMask = 0;
        cd.componentType = kAudioUnitType_MusicDevice;
        cd.componentSubType = kAudioUnitSubType_DLSSynth;
        AUGraphAddNode(graph, &cd, &synthNode);

        cd.componentType = kAudioUnitType_Output;
        cd.componentSubType = kAudioUnitSubType_DefaultOutput;
        AUGraphAddNode(graph, &cd, &outputNode);

        AUGraphOpen(graph);
        AUGraphConnectNodeInput(graph, synthNode, 0, outputNode, 0);
        AUGraphNodeInfo(graph, synthNode, 0, &synthUnit);
        AUGraphInitialize(graph);
        AUGraphStart(graph);
    }
    
    ~Synth()
    {
        AUGraphStop(graph);
        DisposeAUGraph(graph);
    }

    static NAN_METHOD(New)
    {
        Nan::HandleScope scope;
        Synth* synth = new Synth();
        synth->Wrap(info.This());
        return;
    }
    
    static NAN_METHOD(Send)
    {
        Nan::HandleScope scope;
        Synth* synth = Nan::ObjectWrap::Unwrap<Synth>(info.This());
        if (info.Length() == 0 || !info[0]->IsArray()) {
            return Nan::ThrowError("First argument must be an array");
        }

        v8::Local<v8::Object> message = info[0]->ToObject();
        size_t messageLength = message->Get(Nan::New<v8::String>("length").ToLocalChecked())->Int32Value();
        if (messageLength != 3) {
            return Nan::ThrowError("Message array must have 3 elements");
        }

        UInt32 status = message->Get(Nan::New<v8::Integer>(0))->Int32Value();
        UInt32 data1 = message->Get(Nan::New<v8::Integer>(1))->Int32Value();
        UInt32 data2 = message->Get(Nan::New<v8::Integer>(2))->Int32Value();
        MusicDeviceMIDIEvent(synth->synthUnit, status, data1, data2, 0);

        return;
    }
};

Nan::Persistent<v8::FunctionTemplate> Synth::s_ct;

extern "C" {
    void init (v8::Handle<v8::Object> target)
    {
        Synth::Init(target);
    }
    NODE_MODULE(synth, init)
}

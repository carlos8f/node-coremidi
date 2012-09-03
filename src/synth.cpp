#include <node.h>
#include <v8.h>
#include <AudioUnit/AudioUnit.h>
#include <AudioToolbox/AudioToolbox.h>

using namespace node;

class Synth : ObjectWrap
{
private:
    AUGraph graph;
    AudioUnit synthUnit;
public:
    static v8::Persistent<v8::FunctionTemplate> s_ct;
    static void Init(v8::Handle<v8::Object> target)
    {
        v8::HandleScope scope;
        v8::Local<v8::FunctionTemplate> t = v8::FunctionTemplate::New(New);
        
        s_ct = v8::Persistent<v8::FunctionTemplate>::New(t);
        s_ct->InstanceTemplate()->SetInternalFieldCount(1);
        s_ct->SetClassName(v8::String::NewSymbol("Synth"));
        
        NODE_SET_PROTOTYPE_METHOD(s_ct, "send", Send);
        
        target->Set(v8::String::NewSymbol("synth"),
                    s_ct->GetFunction());
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

    static v8::Handle<v8::Value> New(const v8::Arguments& args)
    {
        v8::HandleScope scope;
        Synth* synth = new Synth();
        synth->Wrap(args.This());
        return args.This();
    }
    
    static v8::Handle<v8::Value> Send(const v8::Arguments& args)
    {
        v8::HandleScope scope;
        Synth* synth = ObjectWrap::Unwrap<Synth>(args.This());
        if (args.Length() == 0 || !args[0]->IsArray()) {
            return ThrowException(v8::Exception::TypeError(
                v8::String::New("First argument must be an array")));
        }

        v8::Local<v8::Object> message = args[0]->ToObject();
        size_t messageLength = message->Get(v8::String::New("length"))->Int32Value();

        if (messageLength != 3) {
            return ThrowException(v8::Exception::TypeError(
                v8::String::New("Message array must have 3 elements")));
        }

        UInt32 status = message->Get(v8::Integer::New(0))->Int32Value();
        UInt32 data1 = message->Get(v8::Integer::New(1))->Int32Value();
        UInt32 data2 = message->Get(v8::Integer::New(2))->Int32Value();

        MusicDeviceMIDIEvent(synth->synthUnit, status, data1, data2, 0);

        return scope.Close(v8::Undefined());
    }
};

v8::Persistent<v8::FunctionTemplate> Synth::s_ct;

extern "C" {
    void init (v8::Handle<v8::Object> target)
    {
        Synth::Init(target);
    }
    NODE_MODULE(synth, init)
}

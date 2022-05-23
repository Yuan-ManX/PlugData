/*
 // Copyright (c) 2021-2022 Timothy Schoen and Pierre Guillot
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#pragma once

#include <JuceHeader.h>

#include "PluginProcessor.h"
#include "Sidebar/Sidebar.h"

class Canvas;

namespace pd
{
class Patch;
}

class Box;

enum class Type : size_t
{
    Undefined = 0,
    Text,
    HorizontalSlider,
    VerticalSlider,
    Toggle,
    Number,
    HorizontalRadio,
    VerticalRadio,
    Bang,
    Panel,
    VuMeter,
    Comment,
    AtomNumber,
    AtomSymbol,
    AtomList,
    Array,
    GraphOnParent,
    Message,
    Subpatch,
    Clone,
    Mousepad,
    Mouse,
    Keyboard,
    Picture,
    Scalar,
    NonPatchable,
    Invalid
};

struct ObjectBase : public Component
{
    void* ptr;
    Box* box;
    Canvas* cnv;
    Type type;
    
    ObjectBase(void* obj, Box* parent);

    virtual void showEditor(){};
    virtual void hideEditor(){};

    virtual void updateValue() = 0;
    virtual void updateBounds() = 0;
    virtual void updateDrawables() {};
    
    virtual bool isText() { return false; }
    virtual bool hideInGraph() { return false; }

    virtual void setText(const String&){};

    Type getType()  { return type; }

    // Most objects ignore mouseclicks when locked
    // Objects can override this to do custom locking behaviour
    virtual void lock(bool isLocked)
    {
        setInterceptsMouseClicks(isLocked, isLocked);
    }

    virtual void applyBounds() = 0;
    
    void moveToFront();

    virtual Canvas* getCanvas()
    {
        return nullptr;
    };
    virtual Label* getLabel()
    {
        return nullptr;
    };
    virtual pd::Patch* getPatch()
    {
        return nullptr;
    };

    virtual ObjectParameters getParameters()
    {
        return {};
    };
    
    void closeOpenedSubpatchers();

    String getText();
};

// Class for non-patchable objects
struct NonPatchable : public ObjectBase
{
    NonPatchable(void* obj, Box* parent);
    ~NonPatchable();
    
    virtual void updateValue() {};
    virtual void updateBounds() {};
    virtual void applyBounds() {};
};

struct GUIObject : public ObjectBase, public ComponentListener, public Value::Listener
{
    GUIObject(void* obj, Box* parent);

    ~GUIObject() override;

    void updateValue() override;

    virtual void update(){};

    virtual void initialise();

    void componentMovedOrResized(Component& component, bool moved, bool resized) override;

    void paint(Graphics& g) override;

    String getName() const;

    static Type getType(void* ptr) noexcept;

    static ObjectBase* createGui(void* ptr, Box* parent);

    virtual void checkBounds(){};

    // Get rid of this mess!!
    virtual ObjectParameters defineParameters();
    ObjectParameters getParameters() override;

    pd::Patch* getPatch() override;
    Canvas* getCanvas() override;

    virtual void updateLabel(){};

    virtual float getValue(){ return 0.0f; };

    float getValueOriginal() const noexcept;

    void setValueOriginal(float v);

    float getValueScaled() const noexcept;

    void setValueScaled(float v);

    void startEdition() noexcept;

    void stopEdition() noexcept;

    void mouseDown(const MouseEvent& e) override;
    void mouseUp(const MouseEvent& e) override;

    void valueChanged(Value& value) override{};

    Label* getLabel() override
    {
        return label.get();
    }
    
    void setValue(float value) noexcept;

   protected:

    std::unique_ptr<Label> label;

    bool inspectorWasVisible = false;

    static inline constexpr int maxSize = 1000000;

    PlugDataAudioProcessor& processor;

    std::atomic<bool> edited;
    float value = 0;
    Value min = Value(0.0f);
    Value max = Value(0.0f);
    int width = 6;

    Value sendSymbol;
    Value receiveSymbol;

    Value primaryColour;
    Value secondaryColour;
    Value labelColour;

    Value labelX = Value(0.0f);
    Value labelY = Value(0.0f);
    Value labelHeight = Value(18.0f);

    Value labelText;

    const int atomSizes[7] = {0, 8, 10, 12, 16, 24, 36};
};



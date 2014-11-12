//
//  ScrollBarView.cpp
//  Attach scrollbar on cocos2d::extension::ScrollView.
//  This library also use on cocos2d::extension::TableView.
//
//  Licence: MIT
//  Copyright (c) 2014 Yoshiaki Sugimoto <sugimoto@wnotes.net>
//
//  Referenced: http://blog.naver.com/cloudlevel/50173821064 thanks!
//

#include "ScrollBarView.h"

USING_NS_CC;
USING_NS_CC_EXT;

// Default size macros
#define BAR_FIX_SIZE 10.0f
#define BAR_MIN_SIZE 20.0f

ScrollBarView::ScrollBarView(ScrollView *view, BarType type)
:view(view),
positionType(type),
barFade(true),
fadeDelayTime(0.5f),
barSize(0.0f)
{}

ScrollBarView::~ScrollBarView()
{
    // Remove and clean scroll bar sprite
    removeChild(bar);
    CC_SAFE_DELETE(bar);
}


ScrollBarView* ScrollBarView::create(ScrollView *view, BarType type)
{
    ScrollBarView *sc = new ScrollBarView(view, type);
    if ( ! sc->init() )
    {
        CC_SAFE_DELETE(sc);
        return nullptr;
    }
    
    sc->autorelease();
    return sc;
}

void ScrollBarView::setBarFade(bool barFade)
{
    barFade = barFade;
}

void ScrollBarView::setBarFadeDelayTime(float fadeDelayTime)
{
    fadeDelayTime = fadeDelayTime;
}

bool ScrollBarView::init()
{
    this->setAnchorPoint(Vec2::ZERO);
    if ( ! LayerColor::initWithColor(Color4B(0, 0, 0, 0)) )
    {
        return false;
    }
    
    float offset;
    switch ( positionType )
    {
        case BarType::VERTICAL_IN:
        case BarType::VERTICAL_OUT:
            this->initVertical();
            offset = view->getContentOffset().y;
            break;
            
        case BarType::HORIZONTAL_IN:
        case BarType::HORIZONTAL_OUT:
            this->initHorizontal();
            offset = view->getContentOffset().x;
    }
    
    bool isVisible = ( currentContentSize <= viewFixSize ) ? false: true;
    this->setVisible(isVisible);
    
    this->addChild(bar);
    
    setBarScale();
    setBarPosition(offset);
    
    // Get scrollview parent node and attach this
    Node *parent = view->getParent();
    parent->addChild(this, 1000, 1000); // enough z-order?
    // Set fade-out queue only once
    this->scheduleOnce(schedule_selector(ScrollBarView::fadeOut), fadeDelayTime);
    
    return true;
}

void ScrollBarView::initVertical()
{
    Point position = view->getPosition();
    // Create 9-patch sprite
    Rect imageSize = Rect(0, 0, BAR_FIX_SIZE, BAR_MIN_SIZE);
    Rect insets    = Rect(1, 4, 8, 12);
    bar = Scale9Sprite::create("scrollbar_vertical.png", imageSize, insets);
    
    bar->setAnchorPoint(Vec2(0.5f, 0));
    bar->setPosition(Vec2(bar->getContentSize().width/2, 0));
    
    Size size          = view->cocos2d::Node::getContentSize();
    viewFixSize        = size.height;
    currentContentSize = beforeContentSize = view->getContentSize().height;
    
    // Inner
    if ( positionType == BarType::VERTICAL_IN )
    {
        this->setPosition(position.x + size.width - BAR_FIX_SIZE, position.y);
    }
    // Outer
    else
    {
        this->setPosition(position.x + size.width, position.y);
    }
}

void ScrollBarView::initHorizontal()
{
    Point position = view->getPosition();
    // Create 9-patch sprite
    Rect imageSize = Rect(0, 0, BAR_FIX_SIZE, BAR_MIN_SIZE);
    Rect insets    = Rect(4, 1, 12, 8);
    bar = Scale9Sprite::create("scrollbar_horizontal.png", imageSize, insets);
    
    bar->setAnchorPoint(Vec2(0, 0.5f));
    
    Size size          = view->cocos2d::Node::getContentSize();
    viewFixSize        = size.width;
    currentContentSize = beforeContentSize = view->getContentSize().width;
    
    // Inner
    if ( positionType == BarType::HORIZONTAL_IN )
    {
        this->setPosition(position.x, position.y + (bar->getContentSize().height/2 + 1));
    }
    // Outer
    else
    {
        this->setPosition(position.x, position.y - (bar->getContentSize().height/2 + 1));
    }
    
}

void ScrollBarView::fadeOut(float delta)
{
    if ( ! barFade )
    {
        return;
    }
    
    float x, y;
    switch ( positionType )
    {
        case BarType::VERTICAL_IN:
        case BarType::VERTICAL_OUT:
            x = 0.0f;
            y = 1.0f;
            break;
        case BarType::HORIZONTAL_IN:
        case BarType::HORIZONTAL_OUT:
            x = 1.0f;
            y = 0.0f;
            break;
    }
    
    ScaleTo *action = ScaleTo::create(0.1f, x, y);
    bar->runAction(action);
}

void ScrollBarView::fadeIn()
{
    if ( ! barFade )
    {
        return;
    }
    
    ScaleTo *action = ScaleTo::create(0.05f, 1.0f, 1.0f);
    bar->runAction(action);
}

void ScrollBarView::setBarPosition(float offset)
{
    offset *= -1;
    
    // Calculate bar position
    float pos = (((offset - viewFixSize) / currentContentSize) * viewFixSize) + barSize;
    switch ( positionType )
    {
        case BarType::VERTICAL_IN:
        case BarType::VERTICAL_OUT:
            // Does position be overflow min-y or max-y on scrollview container?
            if ( pos < 0 || pos > (viewFixSize - barSize + 1) )
            {
                return;
            }
            bar->setPosition(Vec2(bar->getPosition().x, pos));
            break;
        case BarType::HORIZONTAL_IN:
        case BarType::HORIZONTAL_OUT:
            // Does position be overflow min-x or max-x on scrollview container?
            if ( pos <= 0 || pos > (viewFixSize - barSize + 1) )
            {
                return;
            }
            bar->setPosition(Vec2(pos, bar->getPosition().y));
            break;
    }
}

void ScrollBarView::setBarScale()
{
    beforeContentSize = currentContentSize;
    
    // Calculate bar scale ( float unit )
    float scale = (viewFixSize / currentContentSize) * viewFixSize;
    barSize = scale;
    
    if ( scale < BAR_MIN_SIZE || scale >= viewFixSize )
    {
        return;
    }
    
    switch ( positionType )
    {
        case BarType::VERTICAL_IN:
        case BarType::VERTICAL_OUT:
            bar->setContentSize(Size(BAR_FIX_SIZE, scale));
            break;
        case BarType::HORIZONTAL_IN:
        case BarType::HORIZONTAL_OUT:
            bar->setContentSize(Size(scale, BAR_FIX_SIZE));
            break;
    }
}

void ScrollBarView::refresh()
{
    if ( bar->getScaleX() == 0.0f || bar->getScaleY() == 0.0f )
    {
        fadeIn();
    }
    
    // Stop all scheduled queues ( e.g. fadeout )
    this->unscheduleAllSelectors();
    // And re-attach fade out schedule queue
    this->scheduleOnce(schedule_selector(ScrollBarView::fadeOut), fadeDelayTime);
    
    float offset;
    switch ( positionType )
    {
        case BarType::VERTICAL_IN:
        case BarType::VERTICAL_OUT:
            offset = view->getContentOffset().y;
            currentContentSize = view->getContentSize().height;
            break;
        case BarType::HORIZONTAL_IN:
        case BarType::HORIZONTAL_OUT:
            offset = view->getContentOffset().x;
            currentContentSize = view->getContentSize().width;
            break;
    }
    
    // Can be visible?
    bool isVisible = ( currentContentSize <= viewFixSize ) ? false : true;
    this->setVisible(isVisible);
    
    // If size is not equal, calculate size and update
    if ( beforeContentSize != currentContentSize )
    {
        setBarScale();
    }
    
    // Set position
    setBarPosition(offset);
}
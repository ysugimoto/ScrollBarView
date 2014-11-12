//
//  ScrollBarView.h
//  Attach scrollbar on cocos2d::extension::ScrollView.
//  This library also use on cocos2d::extension::TableView.
//
//  Licence: MIT
//  Copyright (c) 2014 Yoshiaki Sugimoto <sugimoto@wnotes.net>
//
//  Referenced: http://blog.naver.com/cloudlevel/50173821064 thanks!
//

#ifndef __Lib__ScrollBarView__
#define __Lib__ScrollBarView__

#include "cocos2d.h"
#include "cocos-ext.h"

// Class definition
class ScrollBarView: public cocos2d::LayerColor
{
public:
    
    /**
     * Scrolling bar type enums
     */
    enum class BarType {
        VERTICAL_IN = 1,
        VERTICAL_OUT,
        HORIZONTAL_IN,
        HORIZONTAL_OUT,
    };
    
    /**
     * Constructor
     */
    ScrollBarView(cocos2d::extension::ScrollView *view, BarType type);
    
    /**
     * Destructor
     */
    ~ScrollBarView();
    
    /**
     * Static creation
     */
    static ScrollBarView* create(cocos2d::extension::ScrollView *view, BarType type);
    
    /**
     * Refresh and recalculate bar position/scale
     */
    void refresh();
    
    /**
     * Setter methods
     */
    void setBarFade(bool barFade);
    void setBarSize(float barSize);
    void setBarFadeDelayTime(float fadeDelayTime);
    
private:
    /**
     * Scrolling type enum
     */
    BarType positionType;
    
    /**
     * Delay times untill fade-out bar
     */
    float fadeDelayTime;
    
    /**
     * Enable to fade-out flag
     */
    bool barFade;
    
    /**
     * Stacked bar size
     */
    float barSize;
    
    /**
     * Stacked scroll-view size ( width or height )
     */
    float viewFixSize;
    
    /**
     * Stacked current content size ( width or height )
     */
    float currentContentSize;
    
    /**
     * Stacked before content size ( width or height )
     */
    float beforeContentSize;
    
    /**
     * cocos2d view/sprite properties
     */
    cocos2d::extension::ScrollView *view;
    cocos2d::extension::Scale9Sprite *bar;
    
    
    /**
     * Fade in/out method
     */
    void fadeOut(float delta);
    void fadeIn();
    
    /**
     * Initialize scrollbar
     */
    bool init();
    
    /**
     * Initialize for vertical scroll
     */
    void initVertical();
    
    /**
     * Initialize for horizontal scroll
     */
    void initHorizontal();
    
    /**
     * Recalculate scrollbar position
     */
    void setBarPosition(float offset);
    
    /**
     * Recalculate scrollbar scale
     */
    void setBarScale();
};

#endif /* defined(__Lib__ScrollBarView__) */

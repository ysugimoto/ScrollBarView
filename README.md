ScrollBarView
=============

Attach Scrollbar Node for cocos2d-x's ScrollView/TableView

### Installation

Clone this repository and put `Libs` directory and `Resources/[images]` on your project:

```
git clone git@github.com:ysugimoto/ScrollBarView.git
cp -R ScrollbarView/Libs /path/to/your/cocos-project/
cp -R ScrollbarView/Resoueces/* /path/to/your/cocos-project/Resources/
```

And add to project on Xcode or eclipse.

### Usage

There is sample at https://github.com/ysugimoto/ScrollBarView/blob/master/Classes/HelloWorldScene.cpp

Create ScrollBarView instance and retains (e.g. stack to property),
and `scrollViewDidScroll()`, delegate method, call `refresh()` method simply.

```
…

ScrollView *sc = ScrollView::create(Size([width], [height]));
bar = ScrollBarView::create(sc, ScrollBarView::BarType::VERTICAL_IN);

…

void [CLASS]::scrollViewDidScroll(ScrollView *sc)
{
    // simply call refresh() method.
    bar->refresh();
}
```

Then, automatic calclate and move like Mac OS Scrollbar.

### Licence

MIT Licence.




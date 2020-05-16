/******************************************************************************\
 * Copyright (c) 2004-2020
 *
 * Author(s):
 *  Volker Fischer
 *
 ******************************************************************************
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more 
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
\******************************************************************************/

#pragma once

#include <QFrame>
#include <QPixmap>
#include <QTimer>
#include <QLayout>
#include <QProgressBar>
#include <QStackedLayout>
#include "util.h"
#include "global.h"


/* Definitions ****************************************************************/
// defines for LED level meter CLevelMeterLED
#define NUM_STEPS_LED_BAR                8
#define RED_BOUND_LED_BAR                7
#define YELLOW_BOUND_LED_BAR             5


/* Internal classes declaration ***********************************************/
/* Common private interface to LED class and Bar class */
class CLevelMeterInterface
{
public:
    virtual ~CLevelMeterInterface() {}
    virtual void SetValue ( double dValue ) = 0;
    virtual void Reset () = 0;
    virtual void ClipSet( bool bSet ) = 0;
};

class CLevelMeterBar : public CLevelMeterInterface
{
public:

    CLevelMeterBar ( QWidget* pParent );
    virtual ~CLevelMeterBar();

    void SetValue ( double dValue ) override;
    void Reset() override;
    void ClipSet( bool bSet ) override;

protected:
    QProgressBar* pBar;
    QProgressBar* pClipBar;
};

class CLevelMeterLED : public CLevelMeterInterface
{
public:

    CLevelMeterLED ( QWidget* pParent );
    virtual ~CLevelMeterLED();

    void SetValue( double dValue ) override;
    void Reset() override;
    void ClipSet( bool bSet ) override;

protected:
    class cLED
    {
    public:
        enum ELightColor
        {
            RL_DISABLED,
            RL_GREY,
            RL_GREEN,
            RL_YELLOW,
            RL_RED
        };

        cLED ( QWidget* parent );
        void    setColor ( const ELightColor eNewColor );
        QLabel* getLabelPointer() { return pLEDLabel; }

    protected:
        QPixmap     BitmCubeRoundDisabled;
        QPixmap     BitmCubeRoundGrey;
        QPixmap     BitmCubeRoundGreen;
        QPixmap     BitmCubeRoundYellow;
        QPixmap     BitmCubeRoundRed;

        ELightColor eCurLightColor;
        QLabel*     pLEDLabel;
    };

    CVector<cLED*>     vecpLEDs;
    cLED*              pClipLED;
};

/* Outside interface class ****************************************************/
class CLevelMeter : public QWidget
{
    Q_OBJECT

public:
    enum ELevelMeterType
    {
        MT_LED,
        MT_BAR
    };

    CLevelMeter ( QWidget* parent = nullptr, Qt::WindowFlags f = nullptr );
    virtual ~CLevelMeter();

    void Reset();
    void SetValue ( const double dValue );
    void SetLevelMeterType ( const ELevelMeterType eNType );
    void SetClipTimeout ( int iMilliseconds );

    // Pair another bar with this one. Used to cause action on the paired bar
    // upon mouse event on this bar.
    void SetPairedBar ( CLevelMeter* pBar );

protected slots:
    void ClipReset();

protected:
    virtual void changeEvent ( QEvent* curEvent );
    void mousePressEvent ( QMouseEvent* event ) override;

    QStackedLayout*    pStackedLayout;
    CLevelMeterBar*    pLevelBar;
    CLevelMeterLED*    pLevelLED;
    CLevelMeter*       pPairedBar;

    // Common interface to pLevelBar or pLevelLED, depending on selected skin
    CLevelMeterInterface* pCurrentLevelMeter;

    QTimer*            pClipTimer;

    static constexpr float fClipLimitRatio = 0.95f;
};

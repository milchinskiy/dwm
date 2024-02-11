/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft  = 0;   /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "JetBrainsMono-9:weight=semibold:antialias=true:autohint=false", "JetBrainsMono NFP-11:antialias=true:autohint=false" };
static const char dmenufont[]       = "JetBrainsMono-9:weight=semibold:antialias=true:autohint=false";
static const char col_gray1[]       = "#2e3440";
static const char col_gray2[]       = "#3b4252";
static const char col_gray3[]       = "#e5e9f0";
static const char col_gray4[]       = "#e5e9f0";
static const char col_cyan[]        = "#81A1C1";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_cyan, col_gray1,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "󰈹", "", "", "", "", "", "󰓓", "", "󰭻" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "firefox",  NULL,       NULL,       1 << 0,       0,           -1 },

    { "Brave",    NULL,       NULL,       1 << 1,       0,           -1 },
    { "Chromium", NULL,       NULL,       1 << 1,       0,           -1 },
    { "Vivaldi",  NULL,       NULL,       1 << 1,       0,           -1 },
    { "Code",     NULL,       NULL,       1 << 2,       0,           -1 },
    { "Steam",    NULL,       NULL,       1 << 6,       0,           -1 },
    { "Steam",    NULL,       "Steam",    0,            1,           -1 },
    { "VirtualBox Manager", NULL, NULL,   1 << 7,       0,           -1 },
    { "VirtualBox Machine", NULL, NULL,   1 << 7,       0,           -1 },
    { "Skype",    NULL,       NULL,       1 << 3,       0,           -1 },
    { "ViberPC",  NULL,       NULL,       1 << 8,       0,           -1 },
    { "ViberPC",  NULL,       "ViberPC",  0,            1,           -1 },
    { "TelegramDesktop", NULL, NULL,      1 << 8,       0,           -1 },
    { "feh",      NULL,       NULL,       0,            1,           -1 },
    { "Sxiv",     NULL,       NULL,       0,            1,           -1 },
    { "Galculator", NULL,     NULL,       0,            1,           -1 },
    { "mpv",      NULL,       NULL,       0,            1,           -1 },
    { "vlc",      NULL,       NULL,       0,            1,           -1 },
    { "Enpass",   NULL,       NULL,       0,            1,           -1 },
    { "Enpass Assistant", NULL, NULL,     0,            1,           -1 },
    { "TelegramDesktop", NULL, "Media viewer", 0,       1,           -1 },
    { "St",       NULL,       NULL,       0,            0,           -1 },
    { NULL,       "spterm",   NULL,       0,            1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define ALTKEY Mod1Mask
#define CONTROLKEY ControlMask
#define SHIFTKEY ShiftMask

#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
// static const char *termcmd[]  = { "st", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "160x54", NULL };

static const char *termcmd[]        = { "st", NULL };
static const char *j4dmenu[]        = { "j4-dmenu-desktop", NULL };
static const char *fileManager[]    = { "thunar", NULL };
static const char *screenshoter[]   = { "flameshot", "gui", NULL };
static const char *screenshotLauncher[]   = { "flameshot", "launcher", NULL };

static const char *brightnessUp[]    = { "brightnessctl", "+5%", NULL };
static const char *brightnessDown[]  = { "brightnessctl", "5%-", NULL };

#include "movestack.c"
static Key keys[] = {
	/* modifier                     key        function        argument */
	// { MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	// { MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
    { ALTKEY,                           XK_space,   					spawn,	{.v = j4dmenu } },
    { CONTROLKEY|ALTKEY,                XK_space,   					spawn,	{.v = dmenucmd } },
    { MODKEY|SHIFTKEY,                  XK_Return,  					spawn,  {.v = fileManager } },
    { MODKEY,                           XK_Return,  			        spawn,  {.v = termcmd } },
    { MODKEY,                           XK_space,                       spawn,  SHCMD("sleep 0.05 && atomblocks hit 0") },
    { MODKEY,                           XK_BackSpace,  	        		spawn,  SHCMD("$HOME/.local/bin/powermenu") },
    { MODKEY|ALTKEY|CONTROLKEY,         XK_k,  	        		        spawn,  SHCMD("$HOME/.local/bin/process_killer") },
    { MODKEY|ALTKEY|CONTROLKEY,         XK_v,  	        		        spawn,  SHCMD("$HOME/.local/bin/mk-virt-machine") },
    { MODKEY,                           XK_Print,   					spawn,  {.v = screenshoter } },
    { 0,                             	XK_Print,   					spawn,  SHCMD("flameshot full --clipboard && notify-send \"Screen captured\"") },
    { ALTKEY | SHIFTKEY,                XK_3,   					    spawn,  SHCMD("flameshot full --clipboard && notify-send \"Screen captured\"") },
    { ALTKEY | SHIFTKEY,                XK_4,   					    spawn,  {.v = screenshoter } },
    { ALTKEY | SHIFTKEY,                XK_5,   					    spawn,  {.v = screenshotLauncher } },

    { 0,                             	XF86XK_AudioPlay,   		    spawn,  SHCMD("pgrep -x mocp > /dev/null && mocp -G || playerctl play-pause") },
    { ALTKEY | SHIFTKEY,                XK_t,                  		    spawn,  SHCMD("pgrep -x mocp > /dev/null && mocp -G || playerctl play-pause") },
    { 0,                             	XF86XK_AudioNext,   		    spawn,  SHCMD("pgrep -x mocp > /dev/null && mocp -f || playerctl next") },
    { ALTKEY | SHIFTKEY,               	XK_n,                  		    spawn,  SHCMD("pgrep -x mocp > /dev/null && mocp -f || playerctl next") },
    { 0,                             	XF86XK_AudioPrev,   		    spawn,  SHCMD("pgrep -x mocp > /dev/null && mocp -r || playerctl previous") },
    { ALTKEY | SHIFTKEY,               	XK_p,                 		    spawn,  SHCMD("pgrep -x mocp > /dev/null && mocp -r || playerctl previous") },
    { 0,                             	XF86XK_AudioStop,        		spawn,  SHCMD("pgrep -x mocp > /dev/null && mocp -s || playerctl stop") },
    { ALTKEY | SHIFTKEY,               	XK_s,                      		spawn,  SHCMD("pgrep -x mocp > /dev/null && mocp -s || playerctl stop") },
    { 0,                             	XF86XK_Calculator,  	    	spawn,  SHCMD("galculator") },

    { 0,                             	XF86XK_AudioLowerVolume,     	spawn,  SHCMD("$HOME/.local/bin/volume --dec") },
    { 0,                             	XF86XK_AudioRaiseVolume,    	spawn,  SHCMD("$HOME/.local/bin/volume --inc")},
    { 0,                             	XF86XK_AudioMute,  		        spawn,  SHCMD("$HOME/.local/bin/volume --toggle")},

    { 0,                             	XF86XK_MonBrightnessUp,         spawn,  {.v = brightnessUp } },
    { 0,                             	XF86XK_MonBrightnessDown,       spawn,  {.v = brightnessDown } },

    { ALTKEY,                           XK_F12,                         spawn,  SHCMD("$HOME/.local/bin/wallpaper-rotator") },
    { ALTKEY|SHIFTKEY,                  XK_equal,                       spawn,  SHCMD("$HOME/.local/bin/wallpaper-rotator") },

    { MODKEY,                           XK_F1,                          spawn,  SHCMD("firefox") },
    { ALTKEY|SHIFTKEY,                  XK_1,                           spawn,  SHCMD("firefox") },
    { MODKEY,                           XK_F2,                          spawn,  SHCMD("chromium || brave") },
    { ALTKEY|SHIFTKEY,                  XK_2,                           spawn,  SHCMD("chromium || brave") },
    { MODKEY,                           XK_F3,                          spawn,  SHCMD("code") },
    { ALTKEY|SHIFTKEY,                  XK_3,                           spawn,  SHCMD("code") },
    { MODKEY,                           XK_F7,                          spawn,  SHCMD("steam-runtime || steam") },
    { ALTKEY|SHIFTKEY,                  XK_7,                           spawn,  SHCMD("steam-runtime || steam") },
    { MODKEY,                           XK_F8,                          spawn,  SHCMD("virtualbox") },
    { ALTKEY|SHIFTKEY,                  XK_8,                           spawn,  SHCMD("virtualbox") },
    // { MODKEY,                           XK_F11,     spawn,              SHCMD("loginctl lock-session $XDG_SESSION_ID") },
    // { ALTKEY|SHIFTKEY,                  XK_l,       spawn,              SHCMD("loginctl lock-session $XDG_SESSION_ID") },
    { MODKEY,                           XK_F11,                         spawn,  SHCMD("slock") },
    { ALTKEY|SHIFTKEY,                  XK_l,                           spawn,  SHCMD("slock") },
    { MODKEY,                           XK_F12,                         spawn,  SHCMD("galculator") },

	{ MODKEY,                       XK_grave,  togglescratch,  {.v = scratchpadcmd } },

	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	// { MODKEY,                       XK_space,  setlayout,      {0} },
	{ ALTKEY,                       XK_f,      togglefloating, {0} },
	// { MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	// { MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

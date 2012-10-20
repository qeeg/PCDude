
#include "video.h"
#include "api.h"

Video_t::Video_t()
{
#ifdef USE_NCURSES
	initscr();
#endif
}

Video_t::~Video_t()
{
#ifdef USE_NCURSES
	endwin();
#endif
}

void Video_t::Init()
{
	win.create( sf::VideoMode(720+WW,350+HH), "PCDude - 8086 emulator", sf::Style::Default^sf::Style::Resize );

	fntMono.loadFromFile("gfx/libermono.ttf");
	fntMonoBold.loadFromFile("gfx/libermono-bold.ttf");
	fntSans.loadFromFile("gfx/libersans.ttf");
	guis = sfg::Fixed::Create();
	regs = sfg::Label::Create();
	float tmx, tmy;
	tmx = win.getSize().x - 130;
	tmy = 32.0F;
	guis->Put(regs,sf::Vector2f(tmx,tmy));
#ifdef USE_NCURSES
	w.reset(newwin(0,0,0,0),&delwin);
	wrefresh(w.get());
#endif
}

void Video_t::RefreshScreen()
{
	auto sz = gfxCardGetDisplaySize();
	sf::Uint32 nx = sz[0]+WW;
	sf::Uint32 ny = sz[0]+HH;
	if((nx!=w.getSize().X)||(ny!=w.getSize().Y))
	{
		w.setSize(nx,ny);
	}
	static sf::Clock c;
	static sf::RectangleShape rs(sf::Vector2f(722.0f,352.0f));
	rs.setPosition(sf::Vector2f(23,71));
	rs.setOutlineColor(sf::Color::White);
	rs.setOutlineThickness(1.0f);
	rs.setFillColor(sf::Color::Transparent);
	gui.Update(c.restart().asSeconds());
	win.clear(sf::Color(237,230,197));
	gfxCardRender(win,24,72);
	RefreshRegisters();
	gs.Display(win);
	win.display();
}

void Video_t::RefreshRegisters()
{
	std::stringstream ss;
	ss.flags(std::ios_base::hex);
	using namespace CPU;
#define addout(x) ss << #x ": " << x << "\n"
	addout(ax);
	addout(bx);
	addout(cx);
	addout(dx);
	addout(cs);
	addout(ip);
	addout(ss);
	addout(sp);
	addout(ds);
	addout(es);
	addout(di);
	addout(si);
	addout(bp);
	addout(flags);
#undef addout
	ss << std::flush;
	regs->SetText(ss.str());
}

bool Video_t::PollEvent()
{
	return win.pollEvent(ev);
}

Video_t Video;

/// Null video card definitions
#ifndef USE_MDA
namespace mda
{
void Register() {}
void Unregister() {}
}
#endif
#ifndef USE_HERCULES
namespace HGC
{
void Register() {}
void Unregister() {}
}
#endif
#ifndef USE_CGA
namespace CGA
{
void Register() {}
void Unregister() {}
}
#endif


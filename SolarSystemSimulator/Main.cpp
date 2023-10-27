
#include "Vector3D.h"
#include "celestialBody.h"
#include <vector>
#include "GUI.h"

#include "LinkedList.h"
#include "queue.h"

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
	std::vector<celestialBody*> solarSystem(12);
	celestialBody sun("Sun", 1.988500e30, 696342e3, Vector3D(-6.806783239281648e+08, 1.080005533878725e+09, 6.564012751690170e+06), Vector3D(-1.420511669610689e+01, -4.954714716629277e+00, 3.994237625449041e-01), { 232, 138, 37 , 255});
	celestialBody Mercury("Mercury", 3.302e23, 2439.7e3, Vector3D(6.047855986424127e+06, -6.801800047868888e+10, -5.702742359714534e+09), Vector3D(3.892585189044652e+04, 2.978342247012996e+03, -3.327964151414740e+03), { 112, 109, 107, 255});
	celestialBody Venus("Venus", 4.8685e24, 6051.8e3, Vector3D(-9.435345478592035e+10, 5.350359551033670e+10, 6.131453014410347e+09), Vector3D(-1.726404287724406e+04, -3.073432518238123e+04, 5.741783385280979e-04), { 196, 165, 143, 255 });
	celestialBody Earth("Earth", 5.97219e24, 6371e3, Vector3D(-1.471922101663588e+11, -2.860995816266412e+10, 8.278183193596080e+06), Vector3D(5.427193405797901e+03, -2.931056622265021e+04, 6.575428158157592e-01), { 38, 120, 60, 255 });
	celestialBody Moon("Moon", 7.349e22, 1737.4e3, Vector3D(-1.472343904597218e+11, -2.822578361503422e+10, 1.052790970065631e+07), Vector3D(4.433121605215677e+03, -2.948453614110320e+04, 8.896598225322805e+01), { 171, 169, 167, 255 });
	celestialBody Mars("Mars", 6.4171e23, 3389.5e3, Vector3D(-3.615638921529161e+10, -2.167633037046744e+11, -3.687670305939779e+09), Vector3D(2.481551975121696e+04, -1.816368005464070e+03, -6.467321619018108e+02), { 199, 114, 30, 255 });
	celestialBody Jupiter("Jupiter", 1.89813e27, 69911e3, Vector3D(1.781303138592153e+11, -7.551118436250277e+11, -8.532838524802327e+08), Vector3D(1.255852555185220e+04, 3.622680192790968e+03, -2.958620380112444e+02), { 209, 142, 84, 255 });
	celestialBody Saturn("Saturn", 5.6834e26, 58232e3, Vector3D(6.328646641500651e+11, -1.358172804527507e+12, -1.578520137930810e+09), Vector3D(8.220842186554890e+03, 4.052137378979608e+03, -3.976224719266916e+02), { 212, 169, 131, 255 });
	celestialBody Titan("Titan", 1.34553e23, 2574.7e3, Vector3D(6.332873118527889e+11, -1.357175556995868e+12, -2.134637041453660e+09), Vector3D(3.056877965721629e+03, 6.125612956428791e+03, -9.523587380845593e+02), { 115, 191, 135, 255 });
	celestialBody Uranus("Uranus", 8.6813e25, 25362e3, Vector3D(2.395195786685187e+12, 1.744450959214586e+12, -2.455116324031639e+10), Vector3D(-4.059468635313243e+03, 5.187467354884825e+03, 7.182516236837899e+01), { 190, 232, 237, 255 });
	celestialBody Neptune("Neptune", 1.02413e26, 24622e3, Vector3D(4.382692942729203e+12, -9.093501655486243e+11, -8.227728929479486e+10), Vector3D(1.068410720964204e+03, 5.354959501569486e+03, -1.343918199987533e+02), { 55, 86, 212, 255 });
	celestialBody Probe("Probe", 15e3, 5, (Earth.position + Vector3D(0, -Earth.getRadius() - 10, 0)), (Earth.velocity + Vector3D(0, -30e3, 0)), {200, 86, 150, 255});
	solarSystem[0] = &sun;
	solarSystem[1] = &Mercury;
	solarSystem[2] = &Venus;
	solarSystem[3] = &Earth;
	solarSystem[4] = &Moon;
	solarSystem[5] = &Mars;
	solarSystem[6] = &Jupiter;
	solarSystem[7] = &Saturn;
	solarSystem[8] = &Titan;
	solarSystem[9] = &Uranus;
	solarSystem[10] = &Neptune;
	solarSystem[11] = &Probe;

	Gui::createHWindow("Window", "Class");
	Gui::createDevice();
	Gui::createImGui();
	for (int i = 1; i < 12; i++) {
		LinkedList<Vector3D>* node = new LinkedList<Vector3D>;
		node->element = solarSystem[i]->position;
		Gui::pathDots[i].add(node);
		if (Gui::pathDots[i].size > 80) Gui::pathDots[i].pop();
	}
	
	for (int f = 0; Gui::exit; f++) {
		solarSystem[11]->resolveCollision(solarSystem);
		for (celestialBody* planet : solarSystem) {
			planet->velocity += planet->accelerationDueToGravity(solarSystem);
		}
		for (celestialBody* planet : solarSystem) if(!planet->resolveCollision(solarSystem)) {
			planet->position += planet->velocity;
		}
		if (f >= 10000) {
			for (int i = 1; i < 12; i++) {
				if (Gui::pathDots[i].tail != nullptr && (Gui::pathDots[i].tail->element.distance(solarSystem[i]->position) > 1e10)) {
					LinkedList<Vector3D>* node = new LinkedList<Vector3D>;
					node->element = solarSystem[i]->position;
					Gui::pathDots[i].add(node);
					if (Gui::pathDots[i].size > 60) 
						Gui::pathDots[i].pop();
				}
			}
			Gui::beginRender();
			Gui::Render(solarSystem);
			Gui::endRender();
			f = 0;
		}
	}
	Gui::destroyImGui();
	Gui::destroyDevice();
	Gui::destroyHWindow();
	return 0;
}
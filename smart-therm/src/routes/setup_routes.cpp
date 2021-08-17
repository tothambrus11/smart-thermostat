#include "routes/setup_routes.h"
#include "routes/route_get_temp.h"
#include "routes/route_404.h"

void setupRoutes(){
    routeGetTemp();
    route404();
}
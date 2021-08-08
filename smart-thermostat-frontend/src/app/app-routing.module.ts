import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';
import {LoginComponent} from "./login/login.component";
import {IntervalsComponent} from "./intervals/intervals.component";
import {StatsComponent} from "./stats/stats.component";
import {InformationComponent} from "./information/information.component";
import {LogoutComponent} from "./logout/logout.component";

const routes: Routes = [
  {
    path: 'login',
    component: LoginComponent
  },
  {
    path: 'intervals',
    component: IntervalsComponent
  },
  {
    path: 'stats',
    component: StatsComponent
  },
  {
    path: 'information',
    component: InformationComponent
  },
  {
    path: 'logout',
    component: LogoutComponent
  }
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }

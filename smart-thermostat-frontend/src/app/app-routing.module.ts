import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';
import {LoginComponent} from "./login/login.component";
import {IntervalsComponent} from "./intervals/intervals.component";
import {StatsComponent} from "./stats/stats.component";
import {InformationComponent} from "./information/information.component";
import {LogoutComponent} from "./logout/logout.component";
import {AuthGuard} from './auth.guard';

const routes: Routes = [
  {
    path: 'login',
    component: LoginComponent,
  },
  {
    path: 'intervals',
    component: IntervalsComponent,
    canActivate: [AuthGuard]
  },
  {
    path: 'stats',
    component: StatsComponent,
    canActivate: [AuthGuard]
  },
  {
    path: 'information',
    component: InformationComponent,
    canActivate: [AuthGuard]
  },
  {
    path: 'logout',
    component: LogoutComponent
  },
  {
    path: '',
    pathMatch: 'full',
    redirectTo: 'intervals'
  }
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }

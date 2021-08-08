import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { LoginComponent } from './login/login.component';
import {FormsModule} from "@angular/forms";
import { NavbarComponent } from './navbar/navbar.component';
import { IntervalsComponent } from './intervals/intervals.component';
import { InformationComponent } from './information/information.component';
import { StatsComponent } from './stats/stats.component';
import { LogoutComponent } from './logout/logout.component';
import { TempInputComponent } from './temp-input/temp-input.component';
import { CardComponent } from './card/card.component';
import { CardRowComponent } from './card-row/card-row.component';

@NgModule({
  declarations: [
    AppComponent,
    LoginComponent,
    NavbarComponent,
    IntervalsComponent,
    InformationComponent,
    StatsComponent,
    LogoutComponent,
    TempInputComponent,
    CardComponent,
    CardRowComponent
  ],
    imports: [
        BrowserModule,
        AppRoutingModule,
        FormsModule
    ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }

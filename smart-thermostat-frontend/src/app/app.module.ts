import {BrowserModule} from '@angular/platform-browser';
import {NgModule} from '@angular/core';

import {AppRoutingModule} from './app-routing.module';
import {AppComponent} from './app.component';
import {LoginComponent} from './login/login.component';
import {FormsModule} from '@angular/forms';
import {NavbarComponent} from './navbar/navbar.component';
import {IntervalsComponent} from './intervals/intervals.component';
import {InformationComponent} from './information/information.component';
import {StatsComponent} from './stats/stats.component';
import {LogoutComponent} from './logout/logout.component';
import {TempInputComponent} from './temp-input/temp-input.component';
import {CardComponent} from './card/card.component';
import {CardRowComponent} from './card-row/card-row.component';
import {SwitchComponent} from './switch/switch.component';
import {InputTimeIntervalComponent} from './input-time-interval/input-time-interval.component';
import {DropdownListComponent} from './dropdown-list/dropdown-list.component';
import {InputDatetimeRangeComponent} from './input-datetime-range/input-datetime-range.component';
import {TimeToStringPipe} from './time-to-string.pipe';
import {DateToStringPipe} from './date-to-string.pipe';
import {CheckboxComponent} from './checkbox/checkbox.component';
import {SelectDaysOfWeekComponent} from './select-days-of-week/select-days-of-week.component';
import {IntervalCardComponent} from './interval-card/interval-card.component';
import {AlertComponent} from './alert/alert.component';
import {HTTP_INTERCEPTORS, HttpClient, HttpClientModule} from '@angular/common/http';
import {ConfigService} from './config.service';
import {APP_INITIALIZER} from '@angular/core';
import {TempCircleComponent} from './temp-circle/temp-circle.component';
import {TempPipe} from './temp.pipe';
import {BasicAuthInterceptor} from './auth-interceptor';
import {ErrorInterceptor} from './logout-interceptor';
import { ConnectToAnotherWifiComponent } from './connect-to-another-wifi/connect-to-another-wifi.component';
import { PopupContainerComponent } from './popup-container/popup-container.component';

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
    CardRowComponent,
    SwitchComponent,
    InputTimeIntervalComponent,
    DropdownListComponent,
    InputDatetimeRangeComponent,
    TimeToStringPipe,
    DateToStringPipe,
    CheckboxComponent,
    SelectDaysOfWeekComponent,
    IntervalCardComponent,
    AlertComponent,
    TempCircleComponent,
    TempPipe,
    ConnectToAnotherWifiComponent,
    PopupContainerComponent
  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    FormsModule,
    HttpClientModule
  ],
  providers: [
    {
      provide: APP_INITIALIZER,
      useFactory: ConfigService.factory,
      deps: [HttpClient, ConfigService],
      multi: true
    },
    {provide: HTTP_INTERCEPTORS, useClass: BasicAuthInterceptor, multi: true},
    {provide: HTTP_INTERCEPTORS, useClass: ErrorInterceptor, multi: true},
  ],
  bootstrap: [AppComponent]
})
export class AppModule {
}

import {Component, OnInit} from '@angular/core';
import {TempService} from '../temp.service';
import {SwalService} from '../swal.service';
import {IntervalType, RepetitionFrequency, StoredInterval} from '../interval';
import {daysLocalized, numberToBits} from '../select-days-of-week/select-days-of-week.component';
import {ConfigService} from '../config.service';

@Component({
  selector: 'app-information',
  templateUrl: './information.component.html',
  styleUrls: ['./information.component.scss']
})
export class InformationComponent implements OnInit {
  activeIntervalType = '';
  currentInterval: StoredInterval | null = null;
  RepetitionFrequency = RepetitionFrequency;
  IntervalType = IntervalType;
  nd?: { wifiSSID: string, localIP: string, mac: string, port: number, publicIp: string };
  ap?: { ssid: string, password: string };

  constructor(public tempService: TempService, private swal: SwalService, private configService: ConfigService) {

  }

  async ngOnInit() {
    this.hidePassword = true;
    this.hideAPPassword = true;

    this.tempService.getActiveInterval().then(async c => {
      this.currentInterval = c as any;

      if (this.currentInterval) {
        if (this.currentInterval.repetitionFrequency == RepetitionFrequency.WEEKLY) {
          let isActiveDay: boolean[] = [];
          numberToBits(this.currentInterval.daysOfWeek!, isActiveDay);
          let activeDays = [];
          for (let i = 0; i < isActiveDay.length; i++) {
            if (isActiveDay[i]) {
              activeDays.push(daysLocalized[i]);
            }
          }
          this.weeklyDaysOfWeek = activeDays.join(', ');
        }
      } else {
        this.normalTemp = await this.tempService.getNormalTemp();
      }
    });


    this.tempService.getNetworkData().then(nd => {
      this.nd = nd;
      this.nd!.publicIp = this.configService.serverIP;
    });

    this.tempService.getApCredentials().then(ap=>{
      this.ap = ap;
    })

  }
  weeklyDaysOfWeek?: string;
  normalTemp = 0;
  hidePassword = true;
  hideAPPassword = true;

  reEv() {
    this.tempService.resetEverything().then(() => this.swal.ok());
  }

  reInterval() {
    this.tempService.resetIntervals().then(() => this.swal.ok());
  }

  restart() {
    this.tempService.restart().then(() => this.swal.ok());
  }
}

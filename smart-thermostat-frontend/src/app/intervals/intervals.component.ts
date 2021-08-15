import {Component, OnInit} from '@angular/core';
import {StoredInterval, IntervalType, repetitionFrequenciesLocal, RepetitionFrequency} from '../interval';
import {TempService} from '../temp.service';
import {AlertService} from '../alert.service';

@Component({
  selector: 'app-intervals',
  templateUrl: './intervals.component.html',
  styleUrls: ['./intervals.component.scss']
})
export class IntervalsComponent implements OnInit {

  frequencies = repetitionFrequenciesLocal.hu;

  intervals: StoredInterval[] = [];
  IntervalType = IntervalType;
  RepetitionFrequency = RepetitionFrequency;

  constructor(public intervalService: TempService, public alertService: AlertService) {
  }

  ngOnInit(): void {
    this.intervalService.getIntervalsStoredData().then(ivs => {
      this.intervals = ivs;
    });
  }

  saveNormalTemp(temp: number): void {
    this.intervalService.setNormalTemp(temp);
  }

  changedFrequency(selectedIndex: number, interval: StoredInterval) {
    interval.repetitionFrequency = selectedIndex;
    interval.enabled = false;

    switch (interval.repetitionFrequency) {
      case RepetitionFrequency.NEVER:
        let d = new Date();
        interval.startYear = d.getFullYear();
        interval.startMonth = d.getMonth() + 1;
        interval.startDay = d.getDate();
        interval.startHour = d.getHours();
        interval.startMinute = d.getMinutes();
        interval.endYear = d.getFullYear();
        interval.endMonth = d.getMonth() + 1;
        interval.endDay = d.getDate();
        interval.endHour = d.getHours();
        interval.endMinute = d.getMinutes();
        break;
      case RepetitionFrequency.WEEKLY:
        interval.daysOfWeek = 0;
        break;
      case RepetitionFrequency.DAILY:
        interval.startHour = 0;
        interval.startMinute = 0;
        interval.endHour = 0;
        interval.endMinute = 0;
        break;
    }
  }

  up(currentInterval: StoredInterval) {
    for (let interval of this.intervals) {
      if (interval.order == currentInterval.order - 1) {
        interval.order++;
        currentInterval.order--;
        // todo save
        break;
      }
    }
  }

  down(currentInterval: StoredInterval) {
    for (let interval of this.intervals) {
      if (interval.order == currentInterval.order + 1) {
        interval.order--;
        currentInterval.order++;
        // todo save
        break;
      }
    }
  }

  deleteInterval(interval: StoredInterval) {
    this.alertService.alert({
      title: 'Biztos törölni szeretnéd az intervallumot?',
      message: 'Nem lehet visszavonni, vigyázz!',
      icon: 'delete',
      isWarning: true,
      okButtonText: 'Törlés',
      showOkButton: true,
      showCancelButton: true,
      onOk: () => {
        //TODO implement
      },
    });
  }
}

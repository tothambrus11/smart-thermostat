import {Component, OnInit} from '@angular/core';
import {StoredInterval, IntervalType, repetitionFrequenciesLocal, RepetitionFrequency} from '../interval';
import {TempService} from '../temp.service';
import {AlertService} from '../alert.service';
import {DateToStringPipe} from '../date-to-string.pipe';

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
  normalTemp = 0;

  constructor(public svc: TempService, public alertService: AlertService) {
  }

  ngOnInit(): void {
    this.svc.getIntervalsStoredData().then((data) => {
      this.intervals = data.items;
      this.normalTemp = data.normalTemp;
    });
  }

  saveNormalTemp(temp: number): void {
    this.svc.setNormalTemp(temp);
  }

  changedFrequency(selectedIndex: number, interval: StoredInterval) {
    interval.repetitionFrequency = selectedIndex;
    interval.enabled = false;

    let d = new Date();

    switch (interval.repetitionFrequency) {
      case RepetitionFrequency.NEVER:
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

    this.svc.modifyProperty('enabled', 0, interval.order).then(() => {
      this.svc.modifyProperty('repetitionFrequency', selectedIndex, interval.order);

      switch (interval.repetitionFrequency) {
        case RepetitionFrequency.NEVER:
          let cDate = DateToStringPipe.transform(0, interval.startYear!, interval.startMonth!, interval.startDay!, interval.startHour!, interval.startMinute!);
          this.svc.modifyProperty('startDateTime', cDate, interval.order).then(() => {
            this.svc.modifyProperty('endDateTime', cDate, interval.order);
            console.log(cDate)
          });
          break;
        case RepetitionFrequency.WEEKLY:
          this.svc.modifyProperty('daysOfWeek', 0, interval.order);
          break;
        case RepetitionFrequency.DAILY:
          this.svc.modifyProperty('startTime', '00:00', interval.order).then(() => {
            this.svc.modifyProperty('endTime', '00:00', interval.order);
          });
          break;
      }
    });

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
      onOk: async () => {
        await this.svc.deleteInterval(interval.order);
        console.log('deleted');
        let index = this.intervals.indexOf(interval);
        this.intervals.splice(index, 1);
        for (let i = index; i < this.intervals.length; i++) {
          this.intervals[i].order--;
        }
      },
    });
  }

  onTime1Change($event: string, interval: StoredInterval) {
    const parts = $event.split(':');
    interval.startHour = Number(parts[0]);
    interval.startMinute = Number(parts[1]);
    this.svc.modifyProperty('startTime', $event, interval.order);
  }

  onTime2Change($event: string, interval: StoredInterval) {
    const parts = $event.split(':');
    interval.endHour = Number(parts[0]);
    interval.endMinute = Number(parts[1]);
    this.svc.modifyProperty('endTime', $event, interval.order);
  }


  onDateTime1Change($event: string, interval: StoredInterval) {
    if (!$event) {
      return;
    }
    let parts = $event.split('T');
    let date = parts[0].split('-');
    let time = parts[1].split(':');
    interval.startYear = Number(date[0]);
    interval.startMonth = Number(date[1]);
    interval.startDay = Number(date[2]);
    interval.startHour = Number(time[0]);
    interval.startMinute = Number(time[1]);
    console.log(time);
    this.svc.modifyProperty('startDateTime', $event, interval.order);
  }

  onDateTime2Change($event: string, interval: StoredInterval) {
    if (!$event) {
      return;
    }
    let parts = $event.split('T');
    let date = parts[0].split('-');
    let time = parts[1].split(':');
    interval.endYear = Number(date[0]);
    interval.endMonth = Number(date[1]);
    interval.endDay = Number(date[2]);
    interval.endHour = Number(time[0]);
    interval.endMinute = Number(time[1]);
    this.svc.modifyProperty('endDateTime', $event, interval.order);
  }
}

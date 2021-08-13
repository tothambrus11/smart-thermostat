import {Component, OnInit} from '@angular/core';
import {DisplayedIntervalData, IntervalType, repetitionFrequenciesLocal, RepetitionFrequency} from '../interval';
import {TempService} from '../temp.service';

@Component({
  selector: 'app-intervals',
  templateUrl: './intervals.component.html',
  styleUrls: ['./intervals.component.scss']
})
export class IntervalsComponent implements OnInit {

  frequencies = repetitionFrequenciesLocal.hu;

  intervals: DisplayedIntervalData[] = [];
  IntervalType = IntervalType;
  RepetitionFrequency = RepetitionFrequency;

  constructor(public intervalService: TempService) {
  }

  ngOnInit(): void {
    this.intervalService.getIntervalsStoredData().then(ivs => {
      this.intervals = ivs.map((value, index) => {
        (value as DisplayedIntervalData).order = index;
        return value as DisplayedIntervalData;
      });
    });
  }

  saveNormalTemp(temp: number): void {
    this.intervalService.setNormalTemp(temp);
  }

  changedFrequency(selectedIndex: number, interval: DisplayedIntervalData) {
    interval.repetitionFrequency = selectedIndex;
    interval.enabled = false;

    switch (interval.repetitionFrequency) {
      case RepetitionFrequency.NEVER:
        let d = new Date();
        interval.startYear = d.getFullYear();
        interval.startMonth = d.getMonth()+1;
        interval.startDay = d.getDate();
        interval.startHour = d.getHours();
        interval.startMinute = d.getMinutes();
        interval.endYear = d.getFullYear();
        interval.endMonth = d.getMonth()+1;
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

}

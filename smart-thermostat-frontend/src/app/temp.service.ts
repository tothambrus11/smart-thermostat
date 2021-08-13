import {Injectable} from '@angular/core';
import {IntervalType, RepetitionFrequency, StoredInterval} from './interval';

@Injectable({
  providedIn: 'root'
})
export class TempService {

  normalTemp = 0;

  constructor() {
    this.getNormalTemp().then(t => {
      this.normalTemp = t;
    });
  }

  async updateNormalTemp() {
    this.normalTemp = await this.getNormalTemp();
  }

  setNormalTemp(temp: number): void {
    this.normalTemp = temp;
    localStorage.setItem('normalTemp', temp.toString());
  }

  getNormalTemp(): Promise<number> {
    return new Promise<number>(resolve => {
      resolve(Number(localStorage.getItem('normalTemp')));
    });
  }

  getIntervalsStoredData(): Promise<StoredInterval[]> {
    return new Promise<StoredInterval[]>(resolve => {
      resolve([
        {
          enabled: true,
          temperature: 17.0,
          type: IntervalType.NIGHT,
          startHour: 22,
          startMinute: 10,
          endHour: 6,
          endMinute: 30,
        },
        {
          enabled: false,
          temperature: 24.2,
          type: IntervalType.CUSTOM,
          repetitionFrequency: RepetitionFrequency.DAILY,
          startHour: 12,
          startMinute: 0,
          endHour: 14,
          endMinute: 30,
        },
        {
          enabled: false,
          temperature: 24.2,
          type: IntervalType.CUSTOM,
          repetitionFrequency: RepetitionFrequency.WEEKLY,
          startHour: 12,
          startMinute: 0,
          endHour: 14,
          endMinute: 30,
          daysOfWeek: 4
        },
        {
          enabled: false,
          temperature: 24.2,
          type: IntervalType.CUSTOM,
          repetitionFrequency: RepetitionFrequency.NEVER,
          startYear: 2000,
          startMonth: 1,
          startDay: 4,
          startHour: 12,
          startMinute: 0,
          endYear: 3000,
          endMonth: 2,
          endDay: 3,
          endHour: 14,
          endMinute: 30,
        }
      ]);
    });
  }
}

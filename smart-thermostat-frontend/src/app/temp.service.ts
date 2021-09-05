import {Injectable} from '@angular/core';
import {IntervalType, RepetitionFrequency, StoredInterval} from './interval';
import {HttpClient} from '@angular/common/http';
import {ConfigService} from './config.service';

@Injectable({
  providedIn: 'root'
})
export class TempService {

  normalTemp = 0;
  targetTemp = 0;
  currentTemp = 0;
  isHeating = true;

  constructor(private http: HttpClient, private config: ConfigService) {

    this.getNormalTemp().then(t => {
      this.normalTemp = t;
    });

    this.getTargetTemp().then(t=>{
      this.targetTemp = t;
    });


    if (window.EventSource) {
      this.startEventListening();
    } else {
      this.startHttpTempUpdateInterval();
    }
  }

  startEventListening() {
    let eventSource = new EventSource(`${this.config.serverBaseURl}events`);

    eventSource.addEventListener('temperature', (e: any) => {
      this.currentTemp = Number(e.data);
    });

    eventSource.addEventListener('target_temperature', (e: any) => {
      this.targetTemp = Number(e.data);
    });

    eventSource.addEventListener('is_heating', (e:any)=>{
      this.isHeating = e.data == "1";
    });

    eventSource.addEventListener('open', e => {
      console.log('Events Connected');
    });

    eventSource.addEventListener('error', (e: any) => {
      console.log(e.target);
      if (e.target.readyState !== EventSource.OPEN) {
        console.log('Events Disconnected');
      }
    }, false);
  }

  startHttpTempUpdateInterval() {
    setInterval(async () => {
      this.getCurrentTemp()
        .then(res => {
          this.currentTemp = Number(res);
        })
        .catch(err => {
          console.error(err);
        });
    }, 1000);
  }

  async getCurrentTemp(): Promise<number> {
    let temp = await this.http.get(`${this.config.serverBaseURl}get-temp`, {responseType: 'text'}).toPromise();
    return Number(temp);
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
          order: 1
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
          order: 2
        },
        {
          enabled: false,
          temperature: 24.5,
          type: IntervalType.CUSTOM,
          repetitionFrequency: RepetitionFrequency.WEEKLY,
          startHour: 12,
          startMinute: 0,
          endHour: 14,
          endMinute: 30,
          daysOfWeek: 4,
          order: 3
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
          order: 4
        }
      ]);
    });
  }

  async getTargetTemp(): Promise<number> {
    let temp = await this.http.get(`${this.config.serverBaseURl}get-target-temp`, {responseType: 'text'}).toPromise();
    return Number(temp);
  }

  async getIsHeating(): Promise<boolean> {
    let temp = await this.http.get(`${this.config.serverBaseURl}get-is-heating`, {responseType: 'text'}).toPromise();
    return temp == "1";
  }
}

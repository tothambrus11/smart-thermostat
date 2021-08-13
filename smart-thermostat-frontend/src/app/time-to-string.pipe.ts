import {Pipe, PipeTransform} from '@angular/core';

@Pipe({
  name: 'timeToString'
})
export class TimeToStringPipe implements PipeTransform {

  transform(value: unknown, hour: number, minute: number): string {
    return hour.toString().padStart(2, '0') + ':' + minute.toString().padStart(2, '0');
  }

}

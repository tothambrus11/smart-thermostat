import {Pipe, PipeTransform} from '@angular/core';

@Pipe({
  name: 'dateToString'
})
export class DateToStringPipe implements PipeTransform {

  transform(value: unknown, year: number, month: number, day: number, hour: number, minute: number): string {
    return year.toString().padStart(4, '0') + '-' + month.toString().padStart(2, '0') + '-' + day.toString().padStart(2, '0') + 'T' + hour.toString().padStart(2, '0') + ':' + minute.toString().padStart(2, '0');
  }

}

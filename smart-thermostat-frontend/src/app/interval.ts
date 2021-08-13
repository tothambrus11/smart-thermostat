
export interface DisplayedIntervalData extends StoredInterval {
  order: number;
}

export interface StoredInterval {
  type: IntervalType;
  temperature: number;
  enabled: boolean;
  repetitionFrequency?: RepetitionFrequency;
  daysOfWeek?: number;
  startYear?: number;
  startMonth?: number;
  startDay?: number;
  startHour?: number;
  startMinute?: number;
  endYear?: number;
  endMonth?: number;
  endDay?: number;
  endHour?: number;
  endMinute?: number;
}

export enum IntervalType{
  NIGHT,
  CUSTOM
}

export enum RepetitionFrequency {
  DAILY, WEEKLY, NEVER
}
export const repetitionFrequenciesLocal = {
  "hu": [
    "Naponta",
    "Hetente",
    "Soha"
  ]
}

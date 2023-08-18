package com.igorion.type.json.impl;

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import com.igorion.type.json.IJsonType;

@JsonIgnoreProperties(ignoreUnknown = true)
public abstract class AJsonTypeImpl implements IJsonType {

}

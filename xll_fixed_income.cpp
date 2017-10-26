// xll_fixed_income.cpp - Fixed Income instruments
#include "fms_fixed_income.h"
#include "fms_fixed_income_cash_deposit.h"
#include "G5260.h"

using namespace fms;
using namespace xll;

#ifdef _DEBUG

xll::test xll_test_fixed_income_intrument([](){
    fixed_income::test_fms_fixed_income_instrument();
    fixed_income::test_fms_fixed_income_cash_deposit();
});
#endif // _DEBUG

#define PREFIX L"FIXED.INCOME."

static AddIn xai_fixed_income_instrument(
    Function(XLL_HANDLE, L"?xll_fixed_income_instrument", PREFIX L"INSTRUMENT")
    .Arg(XLL_FP, L"time", L"is and array of times.")
    .Arg(XLL_FP, L"cash", L"is an array of cash flows.")
    .Uncalced()
    .Category(CATEGORY)
    .FunctionHelp(L"Return a handle to a fixed income instrument.")
);
HANDLEX WINAPI xll_fixed_income_instrument(_FP12* pt, _FP12* pf)
{
#pragma XLLEXPORT
    handlex h;

    try {
        ensure (size(*pt) == size(*pf));
        xll::handle<fixed_income::instrument<>> h_(new fixed_income::instrument<>(size(*pt), pt->array, pf->array));
        h = h_.get();
    }
    catch(const std::exception& ex) {
        XLL_ERROR(ex.what());
    }

    return h;
}

static AddIn xai_fixed_income_instrument_size(
    Function(XLL_DOUBLE, L"?xll_fixed_income_instrument_size", PREFIX L"INSTRUMENT.SIZE")
    .Arg(XLL_HANDLE, L"handle", L"is a handle to an instrument.")
    .Category(CATEGORY)
    .FunctionHelp(L"Return the number of times/cash flows of a fixed income instrument.")
);
double WINAPI xll_fixed_income_instrument_size(HANDLEX h)
{
#pragma XLLEXPORT
    double size;

    try {
        xll::handle<fixed_income::instrument<>> h_(h);
        ensure (h_);
        size = h_->size();
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());
        size = std::numeric_limits<double>::quiet_NaN();
    }

    return size;
}

static AddIn xai_fixed_income_instrument_time(
    Function(XLL_FP, L"?xll_fixed_income_instrument_time", PREFIX L"INSTRUMENT.TIME")
    .Arg(XLL_HANDLE, L"handle", L"is handle to an instrument.")
    .Category(CATEGORY)
    .FunctionHelp(L"Return a one row array of cash flow times of a fixed income instrument.")
);
_FP12* WINAPI xll_fixed_income_instrument_time(HANDLEX h)
{
#pragma XLLEXPORT
    static xll::FP12 t;

    try {
        xll::handle<fixed_income::instrument<>> h_(h);
        ensure (h_);
        t.resize(1, h_->size());
        std::copy(h_->time(), h_->time() + h_->size(), begin(t));
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());
        
        return 0;
    }

    return t.get();
}

static AddIn xai_fixed_income_instrument_cash(
    Function(XLL_FP, L"?xll_fixed_income_instrument_cash", PREFIX L"INSTRUMENT.CASH")
    .Arg(XLL_HANDLE, L"handle", L"is handle to an instrument.")
    .Category(CATEGORY)
    .FunctionHelp(L"Return a one row array of cash flows of a fixed income instrument.")
);
_FP12* WINAPI xll_fixed_income_instrument_cash(HANDLEX h)
{
#pragma XLLEXPORT
    static xll::FP12 c;

    try {
        xll::handle<fixed_income::instrument<>> h_(h);
        ensure (h_);
        c.resize(1, h_->size());
        std::copy(h_->cash(), h_->cash() + h_->size(), begin(c));
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());
        
        return 0;
    }

    return c.get();
}